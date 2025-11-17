//
// Created by sexey on 11.11.2025.
//

#include "sdk_dota_view_render.h"

#include "../sdk_signature.h"
#include "debug/debug_output.h"
#include "memory/pattern_scanner.h"

namespace sdk::singleton
{
    FARPROC C_DotaViewRender::onRenderStart() const
    {
        const auto vtable = *static_cast<void***>(this->instance_);
        return reinterpret_cast<FARPROC>(vtable[ON_RENDER_START_VMT_INDEX]);
    }

    bool C_DotaViewRender::initialize()
    {
        const auto func = reinterpret_cast<GetViewRenderInstance_t>(memory::FindPattern(GetModuleHandleA("client.dll"),
                signature::GET_VIEW_RENDER_INSTANCE_FUNC
            )
        );

        if (!func) {
            dbg("Unable to find GetViewRenderInstance() function!");
            return false;
        }

        if (this->instance_ = func(); !this->instance_) {
            dbg("GetViewRenderInstance() returned nullptr!");
            return false;
        }

        dbg("C_DotaViewRender instance at: %p", this->instance_);
        return true;
    }
} // sdk