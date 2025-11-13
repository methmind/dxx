//
// Created by sexey on 11.11.2025.
//

#ifndef DXX_DLC_HOOK_IMPL_ON_RENDER_START_H
#define DXX_DLC_HOOK_IMPL_ON_RENDER_START_H

#include <string>
#include "sdk/sdk_dota_view_render.h"

namespace hook::impl
{
    constexpr std::string ON_RENDER_START_HOOK_SID = "on_render_start";

    void __fastcall hkOnRenderStart(sdk::C_DotaViewRender* self);
} // hook

#endif //DXX_DLC_HOOK_IMPL_ON_RENDER_START_H