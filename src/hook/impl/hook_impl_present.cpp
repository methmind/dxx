//
// Created by sexey on 07.11.2025.
//

#include "hook_impl_present.h"
#include "hook_impl_type.h"

#include "hook/hook_dispatcher.h"
#include "hook/hook_original_invoker.h"
#include "service_locator/service_locator.h"

namespace hook::impl
{
    HRESULT hkPresent(IDXGISwapChain* instance, UINT sync_interval, UINT flags)
    {
        C_ServiceLocator::getInstance<C_HookDispatcher>()->invoke(
            static_cast<hook_id_t>(hook_impl_type_e::PRESENT), instance, sync_interval, flags
        );

        return MH_CALL_ORIGINAL(hkPresent)(instance, sync_interval, flags);
    }
} // hook