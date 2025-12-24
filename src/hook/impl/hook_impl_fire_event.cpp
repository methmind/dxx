//
// Created by sexey on 23.12.2025.
//

#include "hook_impl_fire_event.h"

#include "hook_impl_type.h"
#include "hook/hook_dispatcher.h"
#include "hook/hook_original_invoker.h"
#include "service_locator/service_locator.h"

namespace hook::impl
{
    bool hkFireEvent(void* instance, void* event)
    {
        if (event) {
            C_ServiceLocator::getInstance<C_HookDispatcher>()->invoke<void*, void*>(
                static_cast<hook_id_t>(hook_impl_type_e::FIRE_EVENT), instance, event
            );
        }
        return MH_CALL_ORIGINAL(hkFireEvent)(instance, event);
    }
} // hook