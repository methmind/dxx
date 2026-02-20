//
// Created by sexey on 14.02.2026.
//
module;
#include <cstdint>
#include <d3d11.h>

#include "hook/hook_invoker.h"

export module hook.impl.present;

import service.locator;
import hook.dispatcher;
import hook.type;

namespace hook
{
    export HRESULT __attribute__((__fastcall__)) hkPresent(IDXGISwapChain* swapChain, UINT sync_interval, UINT flags)
    {
        C_ServiceLocator::Get<C_HookDispatcher>()->invoke<static_cast<uint16_t>(hook_type_e::PRESENT)>(
            swapChain, sync_interval, flags
        );

        return MH_CALL_ORIGINAL(hkPresent)(swapChain, sync_interval, flags);
    }
}
