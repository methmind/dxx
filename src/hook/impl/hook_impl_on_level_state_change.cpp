//
// Created by sexey on 22.11.2025.
//

#include "hook_impl_on_level_state_change.h"

#include "hook_impl_type.h"
#include "hook/hook_dispatcher.h"
#include "hook/hook_original_invoker.h"
#include "service_locator/service_locator.h"

namespace hook::impl
{
    void* __fastcall hkOnLevelInit(void* instance, const char* mapName)
    {
        C_ServiceLocator::getInstance<C_HookDispatcher>()->invoke(static_cast<hook_id_t>(hook_impl_type_e::ON_LEVEL_INIT));
        return MH_CALL_ORIGINAL(hkOnLevelInit)(instance, mapName);
    }

    void* __fastcall hkOnLevelShutdown(void* instance)
    {
        C_ServiceLocator::getInstance<C_HookDispatcher>()->invoke(static_cast<hook_id_t>(hook_impl_type_e::ON_LEVEL_SHUTDOWN));
        return MH_CALL_ORIGINAL(hkOnLevelShutdown)(instance);
    }
} // hook