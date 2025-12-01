//
// Created by sexey on 18.11.2025.
//

#ifndef DXX_DLC_SDK_GAME_ENTITY_SYSTEM_H
#define DXX_DLC_SDK_GAME_ENTITY_SYSTEM_H

#include <cstdint>

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
    public:
        using number_of_entities_t = int32_t(__fastcall*)(void* self, uint8_t unk);

        using get_base_entity_t = void*(__fastcall*)(void* self, int32_t index);

    private:
        void* instance_;

        number_of_entities_t numberOfEntities_;
        get_base_entity_t getBaseEntity_;
        void* onAddEntity_;
        void* onRemoveEntity_;

        bool findInstance();

        bool findMethods();

    public:

        [[nodiscard]] void* getOnAddEntityFunc() const { return this->onAddEntity_; }

        [[nodiscard]] void* getOnRemoveEntityFunc() const { return this->onRemoveEntity_; }

        [[nodiscard]] int32_t numberOfEntities() const;

        template<class T = void*>
        T* getBaseEntity(const int32_t index) const {
            return static_cast<T*>(this->getBaseEntity_(this->instance_, index));
        }

        bool initialize();

        C_GameEntitySystem() = default;

        ~C_GameEntitySystem() = default;
    };
} // sdk

#endif //DXX_DLC_SDK_GAME_ENTITY_SYSTEM_H