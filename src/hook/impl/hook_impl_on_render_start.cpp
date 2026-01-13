//
// Created by sexey on 11.11.2025.
//

#include "hook_impl_on_render_start.h"

#include "hook_impl_type.h"
#include "debug/debug_output.h"
#include "hook/hook_dispatcher.h"
#include "hook/hook_original_invoker.h"
#include "service_locator/service_locator.h"

namespace hook::impl
{
    void hkOnRenderStart(void* instance)
    {
        C_ServiceLocator::getInstance<C_HookDispatcher>()->invoke(
            static_cast<hook_id_t>(hook_impl_type_e::ON_RENDER_START)
        );

        MH_CALL_ORIGINAL(hkOnRenderStart)(instance);
    }
} // hook