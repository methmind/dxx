//
// Created by sexey on 07.11.2025.
//

#include "hook_manager.h"
#include "debug/debug_output.h"

namespace hook
{
    MH_STATUS C_HookManager::Create(void* target, void* detour)
    {
        return MH_CreateHook(target, detour, nullptr);
    }

    bool C_HookManager::Initialize()
    {
        if (const auto err = MH_Initialize(); err != MH_OK) {
            dbg("MH_Initialize got:err = %d", err);
            return false;
        }

        return true;
    }

    bool C_HookManager::Enable()
    {
        if (const auto err = MH_EnableHook(MH_ALL_HOOKS); err != MH_OK) {
            dbg("MH_EnableHook got:err = %d", err);
            return false;
        }

        return true;
    }

    void C_HookManager::Disable()
    {
        MH_Uninitialize();
    }
} // hook