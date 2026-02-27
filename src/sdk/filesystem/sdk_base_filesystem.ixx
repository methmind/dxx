//
// Created by sexey on 24.02.2026.
//
module;
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

export module sdk.base_filesystem;

import memory.vmt;

namespace sdk
{
    /*
     * 48 89 54 24 ? 53 55 57 48 81 EC
     * void(__fastcall*)(void* instance, void* handle);
     * "CBaseFileSystem::Close" - xref to this
     */
    constexpr auto CLOSE_VMT_INDEX = 14;

    /*
     * 4C 8B DC 49 89 5B ? 49 89 6B ? 49 89 73 ? 57 48 83 EC ? 49 C7 43 ? ? ? ? ? 48 8D 05 ? ? ? ? 49 89 43 ? 4D 8D 43 ? 48 8D 05 ? ? ? ? 48 8B FA 49 89 43 ? 48 8D 15 ? ? ? ? 0F 10 44 24 ? 48 8B E9 48 8D 0D ? ? ? ? F2 0F 10 4C 24 ? 33 F6 0F 29 44 24 ? F2 0F 11 4C 24 ? FF 15 ? ? ? ? 48 8B D8 48 85 FF 75 ? 4C 8D 05 ? ? ? ? BA ? ? ? ? 48 8B CD E8 ? ? ? ? EB ? 48 C7 C6
     * uint32_t(__fastcall*)(void* instance, void* handle);
     * "CBaseFileSystem::Size" - xref to this
     */
    constexpr auto SIZE_VMT_INDEX = 18;

    /*
     * 44 89 4C 24 ? 4C 89 44 24 ? 48 89 54 24 ? 55 53 56 57 41 56
     * void*(__fastcall*)(void* instance, const char* filename, const char* options, int32_t unk, const char* searchLocation);
     * "CBaseFileSystem::Open" - xref to this
     */
    constexpr auto OPEN_VMT_INDEX = 78;

    /*
     * 4C 8B DC 49 89 5B ? 49 89 6B ? 49 89 73 ? 57 41 56 41 57 48 83 EC ? 49 C7 43 ? ? ? ? ? 48 8D 05 ? ? ? ? 49 89 43 ? 45 8B F0
     * int32_t(__fastcall*)(void* instance, void* buffer, uint32_t bufferSize, uint32_t readSize, void* handle);
     * "CBaseFileSystem::Read" - xref to this
     */
    constexpr auto READ_VMT_INDEX = 79;

    using vf_close_fn = void(__attribute__((__fastcall__))*)(void* instance, void* handle);

    using vf_size_fn = uint32_t(__attribute__((__fastcall__))*)(void* instance, void* handle);

    using vf_open_fn = void*(__attribute__((__fastcall__))*)(void* instance, const char* filename, const char* options, int32_t unk, const char* searchLocation);

    using vf_read_fn = int32_t(__attribute__((__fastcall__))*)(void* instance, void* buffer, uint32_t bufferSize, uint32_t readSize, void* handle);

    struct vhandle_destructor_s
    {
        void* instance;
        void operator()(void* handle) const
        {
            memory::vcall<vf_close_fn, CLOSE_VMT_INDEX>(this->instance, handle);
        }
    };

    export class C_BaseFileSystem
    {
    public:
        bool readFile(const std::string& filePath, std::vector<uint8_t>& output, const char* searchLocation = "game")
        {
            const std::unique_ptr<void, vhandle_destructor_s> handle(memory::vcall<vf_open_fn, OPEN_VMT_INDEX>(this, filePath.c_str(), "r", 0, searchLocation), vhandle_destructor_s{ this });
            if (!handle) {
                return false;
            }

            const auto fileSize = memory::vcall<vf_size_fn, SIZE_VMT_INDEX>(this, handle.get());
            if (!fileSize) {
                return false;
            }

            output.resize(fileSize);
            if (const auto err = memory::vcall<vf_read_fn, READ_VMT_INDEX>(this, output.data(), static_cast<uint32_t>(output.size()), fileSize, handle.get()); !err) {
                return false;
            }

            return true;
        }
    };
}
