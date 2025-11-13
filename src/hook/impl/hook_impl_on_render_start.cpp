//
// Created by sexey on 11.11.2025.
//

#include "hook_impl_on_render_start.h"

#include "debug/debug_output.h"
#include "hook/hook_dispatcher.h"
#include "hook/hook_original_invoker.h"
#include "service_locator/service_locator.h"

namespace hook::impl
{
    void hkOnRenderStart(sdk::C_DotaViewRender* self)
    {
        C_ServiceLocator::getInstance<C_HookDispatcher>()->invoke(ON_RENDER_START_HOOK_SID);
        MH_CALL_ORIGINAL(hkOnRenderStart)(self);
    }
} // hook