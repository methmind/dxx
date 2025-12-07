//
// Created by sexey on 23.11.2025.
//

#include "hook_impl_on_entity_list_change.h"

#include "hook_impl_type.h"
#include "hook/hook_dispatcher.h"
#include "hook/hook_original_invoker.h"
#include "sdk/interface/sdk_base_model_entity.h"
#include "service_locator/service_locator.h"

namespace hook::impl
{
    void* hkOnAddEntity(void* instance, void* entityInstance, int32_t handle)
    {
        const auto result = MH_CALL_ORIGINAL(hkOnAddEntity)(instance, entityInstance, handle);

        C_ServiceLocator::getInstance<C_HookDispatcher>()->invoke<void*>(
            static_cast<hook_id_t>(hook_impl_type_e::ON_ADD_ENTITY),
            entityInstance
        );

        return result;
    }

    void* hkOnRemoveEntity(void* instance, void* entityInstance, int32_t handle)
    {
        C_ServiceLocator::getInstance<C_HookDispatcher>()->invoke<void*>(
            static_cast<hook_id_t>(hook_impl_type_e::ON_REMOVE_ENTITY),
            entityInstance
        );

        return MH_CALL_ORIGINAL(hkOnRemoveEntity)(instance, entityInstance, handle);}
} // hook