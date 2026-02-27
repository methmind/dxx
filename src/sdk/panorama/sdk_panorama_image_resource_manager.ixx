//
// Created by sexey on 27.02.2026.
//
module;
#include <cstdint>
#include <cstring>

export module sdk.panorama.image_resource_manager;

import memory.vmt;
import sdk.panorama.image_proxy_source;

namespace sdk
{
    /*
     * 48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B F2 48 8B E9
     * void*(__fastcall*)(void* instance, void*, void*, const char* path, uint32_t imageFormat, void* imageData);
     */
    constexpr auto LOAD_IMAGE_VMT = 0;

    export enum class image_format_e : uint32_t
    {
        DXT1 = 1,
        DXT5 = 2,
        I8 = 3,
        RGBA8888 = 4,
        R16 = 5,
        RG1616 = 6,
        RGBA16161616 = 7,
        R16F = 8,
        RG1616F = 9,
        RGBA16161616F = 10,
        R32F = 11,
        RG3232F = 12,
        RGB323232F = 13,
        RGBA32323232F = 14,
        JPEG_RGBA8888 = 15,
        PNG_RGBA8888 = 16,
        JPEG_DXT5 = 17,
        PNG_DXT5 = 18,
        BC6H = 19,
        BC7 = 20,
        ATI2N = 21,
        IA88 = 22,
        ETC2 = 23,
        ETC2_EAC = 24,
        R11_EAC = 25,
        RG11_EAC = 26,
        ATI1N = 27,
        BGRA8888 = 28,
    };

    struct image_data_s
    {
        image_data_s()
        {
            memset(this, 0, sizeof(*this));

            m_iWidth = -1;
            m_iHeight = -1;
            m_iUnk1 = -1;
            m_iUnk2 = -1;
            m_flScale = 1.333f;

            m_iUnk3 = 1;
            m_iUnk4 = 1;
        }

        const char* m_szImagePath{}; // 0x0
        int m_iWidth;                // 0x8
        int m_iHeight;               // 0xC
        int m_iUnk1;                 // 0x10
        int m_iUnk2;                 // 0x14
        float m_flScale;             // 0x18
        char padd[0x30]{};           // 0x1C
        int m_iUnk3;                 // 0x4C
        char pading[0x18]{};         // 0x50
        int m_iUnk4;                 // 0x68
        char padi[0x2C]{};           // 0x6C
    };

    using load_image_fn = C_ImageProxySource*(__attribute__((__fastcall__))*)(void* instance, void*, void*, const char* path, image_format_e format, image_data_s* output);

    export class C_ImageResourceManager
    {
    public:
        [[nodiscard]] C_ImageProxySource* loadImage(const char* path, image_format_e imageFormat)
        {
            image_data_s data;
            data.m_szImagePath = path;

            return memory::vcall<load_image_fn, LOAD_IMAGE_VMT>(this, nullptr, nullptr, path, imageFormat, &data);
        }
    };
}
