//
// Created by sexey on 07.11.2025.
//

#include "hook_impl_present.h"

#include "hook/hook_dispatcher.h"
#include "hook/hook_original_invoker.h"
#include "service_locator/service_locator.h"

namespace hook::impl
{
    HRESULT hkPresent(IDXGISwapChain* self, UINT sync_interval, UINT flags)
    {
        C_ServiceLocator::getInstance<C_HookDispatcher>()->invoke(
            PRESENT_HOOK_SID, self, sync_interval, flags
        );

        return MH_CALL_ORIGINAL(hkPresent)(self, sync_interval, flags);
    }
} // hook