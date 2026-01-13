//
// Created by sexey on 07.11.2025.
//

#ifndef DXX_DLC_DX_PRESENT_H
#define DXX_DLC_DX_PRESENT_H

#include <d3dcommon.h>
#include <windows.h>

namespace dx
{
    constexpr auto WND_TARGET_NAME = L"Dota 2";

    constexpr auto IDXGI_PRESENT_VMT_INDEX = 8;

    constexpr D3D_FEATURE_LEVEL FEATURE_LEVELS[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

    FARPROC GetPresentFunction();
} // dx

#endif //DXX_DLC_DX_PRESENT_H