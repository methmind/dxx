//
// Created by sexey on 28.02.2026.
//
module;
#include "hook/hook_invoker.h"

export module hook.impl.level_state_change;

import service.locator;

import hook.dispatcher;
import hook.type;

export namespace hook
{
    void* __attribute__((__fastcall__)) hkOnLevelInit(void* instance, const char* mapName)
    {
        C_ServiceLocator::Get<C_HookDispatcher>()->invoke<static_cast<hook_id_t>(hook_type_e::ON_LEVEL_INIT)>();
        return MH_CALL_ORIGINAL(hkOnLevelInit)(instance, mapName);
    }

    void* __attribute__((__fastcall__)) hkOnLevelShutdown(void* instance)
    {
        C_ServiceLocator::Get<C_HookDispatcher>()->invoke<static_cast<hook_id_t>(hook_type_e::ON_LEVEL_SHUTDOWN)>();
        return MH_CALL_ORIGINAL(hkOnLevelShutdown)(instance);
    }
}
