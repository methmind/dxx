//
// Created by sexey on 03.03.2026.
//
module;
#include <cstdint>

#include "hook/hook_invoker.h"

export module hook.impl.entity_system;

import service.locator;
import hook.dispatcher;
import hook.type;

export namespace hook
{
    void* __attribute__((__fastcall__)) hkOnAddEntity(void* instance, void* entityInstance, const int32_t handle)
    {
        const auto result = MH_CALL_ORIGINAL(hkOnAddEntity)(instance, entityInstance, handle);
        C_ServiceLocator::Get<C_HookDispatcher>()->invoke<static_cast<hook_id_t>(hook_type_e::ON_ENTITY_ADD)>(
            entityInstance
        );

        return result;
    }

    void* __attribute__((__fastcall__)) hkOnRemoveEntity(void* instance, void* entityInstance, const int32_t handle)
    {
        C_ServiceLocator::Get<C_HookDispatcher>()->invoke<static_cast<hook_id_t>(hook_type_e::ON_ENTITY_REMOVE)>(
            entityInstance
        );

        return MH_CALL_ORIGINAL(hkOnRemoveEntity)(instance, entityInstance, handle);
    }
}
