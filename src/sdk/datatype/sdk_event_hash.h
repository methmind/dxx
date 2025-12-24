//
// Created by sexey on 23.12.2025.
//

#ifndef DXX_DLC_SDK_EVENT_HASH_H
#define DXX_DLC_SDK_EVENT_HASH_H

#include <string_view>
#include "../util/sdk_hash_seed.h"
#include "murmur/MurmurHash2.h"

namespace sdk::datatype
{
    struct event_hash_t_s
    {
        uint32_t hash;
        const char* name;

        explicit event_hash_t_s(const std::string_view& eventName)
        {
            this->hash = MurmurHash2(eventName.data(), eventName.size(), util::MURMUR_HASH_SEED);
            this->name = eventName.data();
        }
    };
}

#endif //DXX_DLC_SDK_EVENT_HASH_H