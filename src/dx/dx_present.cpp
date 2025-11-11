//
// Created by sexey on 07.11.2025.
//

#include "dx_present.h"

#include <d3d11.h>
#include <__stddef_offsetof.h>

#include "debug/debug_output.h"

namespace dx
{
    FARPROC GetPresentFunction()
    {
        const auto dotaWnd = FindWindowW(nullptr, WND_TARGET_NAME);
        if (!dotaWnd) {
            dbg("Unable to find Dota 2 window!");
            return nullptr;
        }

        DXGI_SWAP_CHAIN_DESC swapChainDesc{};
        swapChainDesc.BufferCount = 2;
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.OutputWindow = dotaWnd;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.Windowed = true;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        IDXGISwapChain* swapChain;
        ID3D11Device* device;

        if (const auto err = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
            nullptr, 0, FEATURE_LEVELS, std::size(FEATURE_LEVELS), D3D11_SDK_VERSION,
            &swapChainDesc, &swapChain, &device, nullptr, nullptr
        ); err != S_OK) {
            dbg("D3D11CreateDeviceAndSwapChain got:err = %08X", err);
            return nullptr;
        }

        const auto vtable = *reinterpret_cast<void***>(swapChain);
        const auto presentFunction = vtable[IDXGI_PRESENT_VMT_INDEX];

        swapChain->Release();
        device->Release();

        return reinterpret_cast<FARPROC>(presentFunction);
    }
} // dx