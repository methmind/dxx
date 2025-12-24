//
// Created by sexey on 22.12.2025.
//

#ifndef DXX_DLC_SDK_GAME_EVENT_H
#define DXX_DLC_SDK_GAME_EVENT_H

#include <cstdint>
#include <string_view>

namespace sdk::datatype
{
    /*
     * 8B 41 ? 0F BA E0 ? 73 ? 48 8D 41 ? C3 A9 ? ? ? ? 76 ? 48 8B 41 ? C3 48 8D 05 ? ? ? ? C3 CC CC CC CC CC CC CC CC CC CC CC CC CC CC 48 8B C4
     * const char*(__fastcall*)(void* instance);
     */
    constexpr auto GET_EVENT_NAME_VMT_INDEX = 1;

    /*
     * 48 83 EC ? 8B 02 48 83 C1 ? 89 44 24 ? 8B 42 ? 89 44 24 ? 48 8B 42 ? 48 8D 54 24 ? 48 89 44 24 ? E8 ? ? ? ? 48 83 C4 ? C3 CC CC CC 48 89 5C 24 ? 57
     * int32_t(__fastcall*)(void* instance, event_hash_t_s, int32_t default);
     */
    constexpr auto GET_INT_FIELD_VALUE_VMT_INDEX = 7;

    /*
     * 48 83 EC ? 8B 02 48 83 C1 ? 89 44 24 ? 8B 42 ? 89 44 24 ? 48 8B 42 ? 48 8D 54 24 ? 48 89 44 24 ? E8 ? ? ? ? 48 83 C4 ? C3 CC CC CC 33 C9
     * const char*(__fastcall*)(void* instance, event_hash_t_s, const char* default);
     */
    constexpr auto GET_STRING_FIELD_VALUE_VMT_INDEX = 10;

    class C_GameEvent
    {
    public:
        const char* getName();

        int32_t getInt(const std::string_view& fieldName, int32_t def = 0);

        const char* getString(const std::string_view& fieldName, const char* def = nullptr);
    };
}

#endif //DXX_DLC_SDK_GAME_EVENT_H