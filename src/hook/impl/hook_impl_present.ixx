//
// Created by sexey on 14.02.2026.
//
module;
#include <d3d11.h>

#include "hook/hook_invoker.h"

export module hook.impl.present;

import service.locator;
import hook.dispatcher;

namespace hook
{
    export HRESULT __attribute__((__fastcall__)) hkPresent(IDXGISwapChain* instance, UINT sync_interval, UINT flags)
    {
        C_ServiceLocator::Get<C_HookDispatcher>()->invoke<1488>();
        return MH_CALL_ORIGINAL(hkPresent)(instance, sync_interval, flags);
    }
}
