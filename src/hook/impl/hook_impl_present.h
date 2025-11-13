//
// Created by sexey on 07.11.2025.
//

#ifndef DXX_DLC_HOOK_IMPL_PRESENT_H
#define DXX_DLC_HOOK_IMPL_PRESENT_H

#include <dxgi.h>
#include <string>

namespace hook::impl
{
    constexpr std::string PRESENT_HOOK_SID = "present";

    HRESULT __fastcall hkPresent(IDXGISwapChain* self, UINT sync_interval, UINT flags);
} // hook

#endif //DXX_DLC_HOOK_IMPL_PRESENT_H