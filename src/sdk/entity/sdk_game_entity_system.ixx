//
// Created by sexey on 03.03.2026.
//
module;
#include "memory/offset.h"

export module sdk.game_entity_system;

import sdk.entity.identity;
import sdk.entity.instance;
import sdk.handle;

namespace sdk
{
    /*
     * 48 89 74 24 ? 57 48 83 EC ? 41 B9 ? ? ? ? 41 8B C0 41 23 C1 48 8B F2 41 83 F8 ? 48 8B F9 44 0F 45 C8 41 81 F9 ? ? ? ? 73 ? FF 81
     * void*(__fastcall*)(void* instance, void* entityInstance, uint32_t handle);
     */
    constexpr auto ON_ADD_ENTITY_VMT_INDEX = 15;

    /*
     * 48 89 74 24 ? 57 48 83 EC ? 41 B9 ? ? ? ? 41 8B C0 41 23 C1 48 8B F2 41 83 F8 ? 48 8B F9 44 0F 45 C8 41 81 F9 ? ? ? ? 73 ? FF 89
     * void*(__fastcall*)(void* instance, void* entityInstance, uint32_t handle);
     */
    constexpr auto ON_REMOVE_ENTITY_VMT_INDEX = 16;

    constexpr auto MAX_ENTITIES_IN_CHUNK = 512;

    constexpr auto MAX_CHUNKS_COUNT = 64;

    constexpr auto MAX_TOTAL_ENTITIES = MAX_ENTITIES_IN_CHUNK * MAX_CHUNKS_COUNT;

    struct entity_identities_chunk_s
    {
        C_EntityIdentity identities[MAX_ENTITIES_IN_CHUNK]{};
    };

    export class C_GameEntitySystem
    {
    public:
        [[nodiscard]] void* getOnAddEntityFunc()
        {
            const auto vtable = *reinterpret_cast<void***>(this);
            return vtable[ON_ADD_ENTITY_VMT_INDEX];
        }

        [[nodiscard]] void* getOnRemoveEntityFunc()
        {
            const auto vtable = *reinterpret_cast<void***>(this);
            return vtable[ON_REMOVE_ENTITY_VMT_INDEX];
        }

        template<typename func_t>
        void iterateEntities(func_t callback)
        {
            const auto chunks = getIdentitiesChunks();
            for (auto i = 0; i < MAX_CHUNKS_COUNT; ++i) {
                const auto chunk = chunks[i];
                if (!chunk) {
                    continue;
                }

                for (auto j = 0; j < MAX_ENTITIES_IN_CHUNK; ++j) {
                    auto& identity = chunk->identities[j];
                    if (!identity.getAssignedEntity()) {
                        continue;
                    }

                    const auto globalIndex = i * MAX_ENTITIES_IN_CHUNK + j;
                    if (const C_BaseEntityHandle identityHandle(identity.getEntityHandle());
                        identityHandle.getEntryIndex() != globalIndex) {
                        continue;
                        }

                    if constexpr (std::is_same_v<std::invoke_result_t<func_t, C_EntityInstance*>, bool>) {
                        if (!callback(static_cast<C_EntityInstance*>(identity.getAssignedEntity()))) {
                            return;
                        }
                    } else {
                        callback(static_cast<C_EntityInstance*>(identity.getAssignedEntity()));
                    }
                }
            }
        }
    private:
        OFFSET_INLINE(entity_identities_chunk_s**, getIdentitiesChunks, 0x10);
    };
}
