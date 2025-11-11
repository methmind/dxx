//
// Created by sexey on 11.11.2025.
//

#include "hook_impl_on_render_start.h"

#include "debug/debug_output.h"
#include "hook/hook_original_invoker.h"

namespace hook::impl
{
    void hkOnRenderStart(sdk::C_DotaViewRender* self)
    {
        //todo some on_render logic (for lua)
        MH_CALL_ORIGINAL(hkOnRenderStart)(self);
    }
} // hook