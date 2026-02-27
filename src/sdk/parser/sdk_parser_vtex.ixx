//
// Created by sexey on 26.02.2026.
//
module;
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <optional>
#include <span>
#include <string_view>
#include <vector>

#include "lz4.h"
#include "s3tc/s3tc.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

export module sdk.parser.vtex;

namespace sdk
{
    constexpr std::string_view YCOCG_TAG = "Texture Compiler Version Image YCoCg Conversion";
    constexpr size_t EXTRA_DATA_ENTRY_SIZE = 12;
    constexpr uint8_t PNG_MAGIC[] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };

    template <typename T>
    T ReadAt(const uint8_t* data, size_t offset)
    {
        T value;
        memcpy(&value, data + offset, sizeof(T));

        return value;
    }

    enum class vtex_format_e : uint8_t
    {
        DXT1           = 1,
        DXT5           = 2,
        RGBA8888       = 4,
        PNG_RGBA8888   = 16,
    };

    enum class vtex_extra_data_e : uint32_t
    {
        UNKNOWN             = 0,
        METADATA            = 1,
        COMPRESSED_MIP_SIZE = 2,
        CUBEMAP_RADIANCE_SH = 3,
        SHEET               = 4,
    };

    struct vtex_result_s
    {
        uint16_t width{};
        uint16_t height{};
        std::vector<uint8_t> pixels; // RGBA8888
    };

    #pragma pack(push, 1)
    struct vrf_header_s
    {
        uint32_t fileSize;
        uint16_t headerVersion;
        uint16_t version;
        uint32_t blockOffset;
        uint32_t blockCount;
    };

    struct vrf_block_info_s
    {
        char magic[4];
        uint32_t offset;
        uint32_t size;
    };

    struct vtex_data_header_s
    {
        uint16_t version;
        uint16_t flags;
        float reflectivity[4];
        uint16_t width;
        uint16_t height;
        uint16_t depth;
        vtex_format_e format;
        uint8_t numMipLevels;
        uint32_t picmip0Res;
        uint32_t extraDataOffset;
        uint32_t extraDataCount;
    };
    #pragma pack(pop)

    struct vtex_extra_info_s
    {
        bool isLZ4{};
        std::vector<int32_t> compressedMips;
        uint16_t actualWidth{};
        uint16_t actualHeight{};
        size_t textureDataStart{};
    };

    void ConvertYCoCgToRGB(std::vector<uint8_t>& rgbaPixels)
    {
        for (size_t i = 0; i + 3 < rgbaPixels.size(); i += 4) {
            const float scale = 1.0f / ((static_cast<float>(rgbaPixels[i + 2]) / 8.0f) + 1.0f);
            const float co = ((static_cast<float>(rgbaPixels[i + 0]) / 255.0f) - 0.5f) * scale;
            const float cg = ((static_cast<float>(rgbaPixels[i + 1]) / 255.0f) - 0.5f) * scale;
            const float y  = static_cast<float>(rgbaPixels[i + 3]) / 255.0f;

            const float r = y + co - cg;
            const float g = y + cg;
            const float b = y - co - cg;

            rgbaPixels[i + 0] = static_cast<uint8_t>(std::clamp(r * 255.0f, 0.0f, 255.0f));
            rgbaPixels[i + 1] = static_cast<uint8_t>(std::clamp(g * 255.0f, 0.0f, 255.0f));
            rgbaPixels[i + 2] = static_cast<uint8_t>(std::clamp(b * 255.0f, 0.0f, 255.0f));
            rgbaPixels[i + 3] = 255;
        }
    }

    std::vector<uint8_t> ConvertPackedToRGBA(const std::vector<uint32_t>& packed, uint16_t w, uint16_t h)
    {
        const size_t count = std::min(packed.size(), static_cast<size_t>(w) * h);
        std::vector<uint8_t> out(count * 4);
        for (size_t i = 0; i < count; ++i) {
            out[i * 4 + 0] = static_cast<uint8_t>((packed[i] >> 24) & 0xFF);
            out[i * 4 + 1] = static_cast<uint8_t>((packed[i] >> 16) & 0xFF);
            out[i * 4 + 2] = static_cast<uint8_t>((packed[i] >> 8) & 0xFF);
            out[i * 4 + 3] = static_cast<uint8_t>(packed[i] & 0xFF);
        }

        return out;
    }

    void CropInPlace(vtex_result_s& result, uint16_t targetW, uint16_t targetH)
    {
        if (targetW == 0 || targetH == 0) {
            return;
        }

        if (targetW == result.width && targetH == result.height) {
            return;
        }

        if (targetW > result.width || targetH > result.height) {
            return;
        }

        std::vector<uint8_t> cropped(static_cast<size_t>(targetW) * targetH * 4);
        for (size_t y = 0; y < targetH; ++y) {
            memcpy(
                cropped.data() + y * targetW * 4,
                result.pixels.data() + y * result.width * 4,
                targetW * 4
            );
        }

        result.pixels = std::move(cropped);
        result.width  = targetW;
        result.height = targetH;
    }

    bool IsYCoCg(std::span<const uint8_t> content)
    {
        return std::ranges::search(content, YCOCG_TAG).begin() != content.end();
    }

    std::span<const uint8_t> FindDataBlock(std::span<const uint8_t> content)
    {
        if (content.size() < sizeof(vrf_header_s)) {
            return {};
        }

        const auto hdr = reinterpret_cast<const vrf_header_s*>(content.data());
        const size_t tableOffset = 8 + hdr->blockOffset;

        for (uint32_t i = 0; i < hdr->blockCount; ++i)
        {
            const size_t entryPos = tableOffset + i * sizeof(vrf_block_info_s);
            if (entryPos + sizeof(vrf_block_info_s) > content.size()) {
                break;
            }

            const auto entry = reinterpret_cast<const vrf_block_info_s*>(content.data() + entryPos);
            if (std::string_view(entry->magic, 4) != "DATA") {
                continue;
            }

            const size_t absOffset = (entryPos + 4) + entry->offset;
            if (absOffset >= content.size()) {
                continue;
            }

            return content.subspan(absOffset);
        }

        return {};
    }

    vtex_extra_info_s ParseExtraData(const vtex_data_header_s* hdr, std::span<const uint8_t> dataBlock)
    {
        vtex_extra_info_s info{};
        info.textureDataStart = sizeof(vtex_data_header_s);

        if (hdr->picmip0Res > 0) {
            info.actualWidth  = static_cast<uint16_t>(hdr->picmip0Res & 0xFFFF);
            info.actualHeight = static_cast<uint16_t>(hdr->picmip0Res >> 16);
        }

        constexpr size_t kExtraDataFieldPos = offsetof(vtex_data_header_s, extraDataOffset);
        const size_t entriesStart = kExtraDataFieldPos + hdr->extraDataOffset;

        for (uint32_t i = 0; i < hdr->extraDataCount; ++i) {
            const size_t ep = entriesStart + i * EXTRA_DATA_ENTRY_SIZE;
            if (ep + EXTRA_DATA_ENTRY_SIZE > dataBlock.size()) {
                break;
            }

            const auto type      = ReadAt<uint32_t>(dataBlock.data(), ep);
            const auto rawOffset = ReadAt<uint32_t>(dataBlock.data(), ep + 4);
            const auto size      = ReadAt<uint32_t>(dataBlock.data(), ep + 8);
            const size_t payloadPos = (ep + 4) + rawOffset;

            if (payloadPos + size > info.textureDataStart) {
                info.textureDataStart = payloadPos + size;
            }

            if (type == static_cast<uint32_t>(vtex_extra_data_e::COMPRESSED_MIP_SIZE)) {
                if (payloadPos + 4 > dataBlock.size()) {
                    continue;
                }

                info.isLZ4 = (ReadAt<uint32_t>(dataBlock.data(), payloadPos) != 0);
                for (uint8_t m = 0; m < hdr->numMipLevels; ++m) {
                    const size_t pos = payloadPos + 4 + m * 4;
                    if (pos + 4 > dataBlock.size()) {
                        break;
                    }

                    info.compressedMips.push_back(ReadAt<int32_t>(dataBlock.data(), pos));
                }
            } else if (type == static_cast<uint32_t>(vtex_extra_data_e::CUBEMAP_RADIANCE_SH) && size >= 8) {
                info.actualWidth  = ReadAt<uint16_t>(dataBlock.data(), payloadPos + 2);
                info.actualHeight = ReadAt<uint16_t>(dataBlock.data(), payloadPos + 4);
            }
        }

        return info;
    }

    std::optional<vtex_result_s> DecodeDXT(
        const vtex_data_header_s* hdr, std::span<const uint8_t> dataBlock,
        const vtex_extra_info_s& extra, bool isYCoCg
    )
    {
        vtex_result_s result{};
        result.width  = hdr->width;
        result.height = hdr->height;

        const int32_t bytesPerBlock = (hdr->format == vtex_format_e::DXT5) ? 16 : 8;
        const int32_t blocksX = (result.width  + 3) / 4;
        const int32_t blocksY = (result.height + 3) / 4;
        const int32_t uncompSize = blocksX * blocksY * bytesPerBlock;

        std::vector<uint8_t> dxtBlocks(uncompSize);
        if (!extra.compressedMips.empty()) {
            const int32_t mip0Compressed = extra.compressedMips[0];
            if (mip0Compressed <= 0 || static_cast<size_t>(mip0Compressed) > dataBlock.size()) {
                return std::nullopt;
            }

            const size_t mip0Start = dataBlock.size() - mip0Compressed;

            if (extra.isLZ4 && mip0Compressed != uncompSize) {
                const auto decoded = LZ4_decompress_safe(
                    reinterpret_cast<const char*>(dataBlock.data() + mip0Start),
                    reinterpret_cast<char*>(dxtBlocks.data()),
                    mip0Compressed, uncompSize
                );

                if (decoded < 0) {
                    return std::nullopt;
                }
            }
            else {
                memcpy(dxtBlocks.data(), dataBlock.data() + mip0Start, std::min<size_t>(mip0Compressed, uncompSize));
            }
        }
        else {
            if (dataBlock.size() < static_cast<size_t>(uncompSize)) {
                return std::nullopt;
            }

            const size_t start = dataBlock.size() - uncompSize;
            memcpy(dxtBlocks.data(), dataBlock.data() + start, uncompSize);
        }

        std::vector<uint32_t> imgBuf(static_cast<size_t>(result.width) * result.height);
        if (hdr->format == vtex_format_e::DXT5) {
            BlockDecompressImageDXT5(result.width, result.height, dxtBlocks.data(),
                reinterpret_cast<unsigned long*>(imgBuf.data()));
        }
        else {
            BlockDecompressImageDXT1(result.width, result.height, dxtBlocks.data(),
                reinterpret_cast<unsigned long*>(imgBuf.data()));
        }

        result.pixels = ConvertPackedToRGBA(imgBuf, result.width, result.height);
        if (isYCoCg) {
            ConvertYCoCgToRGB(result.pixels);
        }

        CropInPlace(result, extra.actualWidth, extra.actualHeight);
        return result;
    }

    std::optional<vtex_result_s> DecodeRGBA(const vtex_data_header_s* hdr, std::span<const uint8_t> dataBlock, size_t textureDataStart)
    {
        vtex_result_s result{};
        result.width  = hdr->width;
        result.height = hdr->height;

        if (const auto it = std::ranges::search(dataBlock, PNG_MAGIC).begin(); it != dataBlock.end()) {
            int32_t w, h, ch;
            const auto pngSize = static_cast<int>(std::distance(it, dataBlock.end()));
            if (auto* px = stbi_load_from_memory(&(*it), pngSize, &w, &h, &ch, 4)) {
                result.width  = static_cast<uint16_t>(w);
                result.height = static_cast<uint16_t>(h);
                result.pixels.assign(px, px + static_cast<size_t>(w) * h * 4);
                stbi_image_free(px);
            }
        }
        else if (hdr->format == vtex_format_e::RGBA8888) {
            if (const auto expected = static_cast<size_t>(result.width) * result.height * 4;
                dataBlock.size() >= textureDataStart + expected) {
                result.pixels.assign(
                    dataBlock.data() + textureDataStart,
                    dataBlock.data() + textureDataStart + expected
                );
            }
        }

        if (result.pixels.empty()) {
            return std::nullopt;
        }

        return result;
    }

    export std::optional<vtex_result_s> DecodeVtexPNG(std::span<const uint8_t> content)
    {
        const bool isYCoCg = IsYCoCg(content);
        const auto dataBlock = FindDataBlock(content);
        if (dataBlock.size() < sizeof(vtex_data_header_s)) {
            return std::nullopt;
        }

        const auto hdr = reinterpret_cast<const vtex_data_header_s*>(dataBlock.data());
        const auto extra = ParseExtraData(hdr, dataBlock);

        switch (hdr->format)
        {
        case vtex_format_e::PNG_RGBA8888:
        case vtex_format_e::RGBA8888:
            return DecodeRGBA(hdr, dataBlock, extra.textureDataStart);
        case vtex_format_e::DXT1:
        case vtex_format_e::DXT5:
            return DecodeDXT(hdr, dataBlock, extra, isYCoCg);
        default:
            return std::nullopt;
        }
    }
}
