//
// Created by sexey on 18.11.2025.
//

#ifndef DXX_DLC_SDK_GAME_ENTITY_SYSTEM_H
#define DXX_DLC_SDK_GAME_ENTITY_SYSTEM_H

#include "sdk/custom/sdk_entity_identities_chunk.h"
#include "sdk/interface/sdk_entity_instance.h"

namespace sdk::singleton
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

    class C_GameEntitySystem
    {
    private:
        void* instance_;
        custom::entity_identities_chunk_s** identitiesChunks_;

        bool findInstance();

        void* getBaseEntityImpl(int32_t index) const;

        auto getVtable() const { return *static_cast<void***>(this->instance_); }

    public:

        [[nodiscard]] void* getOnAddEntityFunc() const { return getVtable()[ON_ADD_ENTITY_VMT_INDEX]; }

        [[nodiscard]] void* getOnRemoveEntityFunc() const { return getVtable()[ON_REMOVE_ENTITY_VMT_INDEX]; }

        template<class T = void*>
        T* getBaseEntity(const int32_t index) const {
            return static_cast<T*>(getBaseEntityImpl(index));
        }

        template<typename func_t>
        void iterateEntities(func_t callback) const
        {
            for (auto i = 0; i < custom::MAX_CHUNKS_COUNT; ++i) {
                const auto chunk = this->identitiesChunks_[i];
                if (!chunk) {
                    continue;
                }

                for (auto j = 0; j < custom::MAX_ENTITIES_IN_CHUNK; ++j) {
                    auto& identity = chunk->identities[j];
                    if (!identity.getAssignedEntity()) {
                        continue;
                    }

                    const auto globalIndex = i * custom::MAX_ENTITIES_IN_CHUNK + j;
                    if (const util::C_BaseEntityHandle identityHandle(identity.getEntityHandle());
                        identityHandle.getEntryIndex() != globalIndex) {
                        continue;
                    }

                    if constexpr (std::is_same_v<std::invoke_result_t<func_t, iface::C_EntityInstance*>, bool>) {
                        if (!callback(static_cast<iface::C_EntityInstance*>(identity.getAssignedEntity()))) {
                            return;
                        }
                    } else {
                        callback(static_cast<iface::C_EntityInstance*>(identity.getAssignedEntity()));
                    }
                }
            }
        }

        bool initialize();

        C_GameEntitySystem() = default;

        ~C_GameEntitySystem() = default;
    };
} // sdk

#endif //DXX_DLC_SDK_GAME_ENTITY_SYSTEM_H