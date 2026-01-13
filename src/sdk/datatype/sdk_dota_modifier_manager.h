//
// Created by sexey on 05.12.2025.
//

#ifndef DXX_DLC_SDK_DOTA_MODIFIER_MANAGER_H
#define DXX_DLC_SDK_DOTA_MODIFIER_MANAGER_H

#include "sdk_base_entity.h"
#include "sdk_dota_buff.h"
#include "sdk/memory/sdk_memory_utl_vector.h"

namespace sdk::datatype
{
    class C_DotaModifierManager
    {
    private:
        void* vtable_;
        C_BaseEntity* assignedEntity_;
        memory::C_UtlVector<C_DotaBuff*> modifiers_;

    public:

        [[nodiscard]] C_BaseEntity* getAssignedEntity() const { return this->assignedEntity_; }

        [[nodiscard]] memory::C_UtlVector<C_DotaBuff*>& getModifiers() { return this->modifiers_; }

        C_DotaModifierManager() : vtable_(nullptr), assignedEntity_(nullptr) {}

        ~C_DotaModifierManager() = default;
    };
}

#endif //DXX_DLC_SDK_DOTA_MODIFIER_MANAGER_H