//
// Created by sexey on 22.12.2025.
//

#include "sdk_game_event.h"

#include "sdk_event_hash.h"
#include "memory/vmt_call.h"

namespace sdk::datatype
{
    const char* C_GameEvent::getName()
    {
        return memory::vmt::call<const char*(__fastcall*)(void*), GET_EVENT_NAME_VMT_INDEX>(this);
    }

    int32_t C_GameEvent::getInt(const std::string_view& fieldName, const int32_t def)
    {
        return memory::vmt::call<int32_t(__fastcall*)(void*, event_hash_t_s, int32_t), GET_INT_FIELD_VALUE_VMT_INDEX>(
            this, event_hash_t_s(fieldName), def
        );
    }

    const char* C_GameEvent::getString(const std::string_view& fieldName, const char* def)
    {
        return memory::vmt::call<const char*(__fastcall*)(void*, event_hash_t_s, const char*), GET_STRING_FIELD_VALUE_VMT_INDEX>(
            this, event_hash_t_s(fieldName), def
        );
    }
}
