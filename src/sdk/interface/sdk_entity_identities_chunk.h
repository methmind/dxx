//
// Created by sexey on 07.12.2025.
//

#ifndef DXX_DLC_SDK_IDENTITIES_CHUNK_H
#define DXX_DLC_SDK_IDENTITIES_CHUNK_H

#include "sdk/interface/sdk_entity_identity.h"

namespace sdk::iface
{
    constexpr auto MAX_ENTITIES_IN_CHUNK = 512;

    constexpr auto MAX_CHUNKS_COUNT = 64;

    constexpr auto MAX_TOTAL_ENTITIES = MAX_ENTITIES_IN_CHUNK * MAX_CHUNKS_COUNT;

    struct entity_identities_chunk_s
    {
        C_EntityIdentity identities[MAX_ENTITIES_IN_CHUNK];
    };
}

#endif //DXX_DLC_SDK_IDENTITIES_CHUNK_H