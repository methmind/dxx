//
// Created by sexey on 18.11.2025.
//

#ifndef DXX_DLC_SDK_GAME_ENTITY_SYSTEM_H
#define DXX_DLC_SDK_GAME_ENTITY_SYSTEM_H

#include <cstdint>

namespace sdk::singleton
{
    class C_GameEntitySystem
    {
    public:
        using number_of_entities_t = int32_t(__fastcall*)(void* self, uint8_t unk);

        using get_base_entity_t = void*(__fastcall*)(void* self, int32_t index);

    private:
        void* instance_;

        number_of_entities_t numberOfEntities_;
        get_base_entity_t getBaseEntity_;

        bool findInstance();

        bool findMethods();

    public:

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