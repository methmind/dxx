//
// Created by sexey on 13.02.2026.
//
module;
#include <d3d11.h>
#include <iterator>
#include <debug/debug_output.h>

export module dx;

namespace dx
{
    constexpr auto TARGET_WINDOW_NAME = "Dota 2";

    constexpr auto IDXGI_PRESENT_FUNCTION_INDEX = 8;

    constexpr D3D_FEATURE_LEVEL FEATURE_LEVELS[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

    export void* GetPresentFunction()
    {
        const auto targetWnd = FindWindowA(nullptr, TARGET_WINDOW_NAME);
        if (!targetWnd) {
            dbg("FindWindowA got:err = Unable to find {} window!", TARGET_WINDOW_NAME);
            return nullptr;
        }

        DXGI_SWAP_CHAIN_DESC swapChainDesc{};
        swapChainDesc.BufferCount = 2;
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.OutputWindow = targetWnd;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.Windowed = true;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        IDXGISwapChain* swapChain;
        ID3D11Device* device;

        if (const auto err = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
            nullptr, 0, FEATURE_LEVELS, std::size(FEATURE_LEVELS), D3D11_SDK_VERSION,
            &swapChainDesc, &swapChain, &device, nullptr, nullptr
        ); err != S_OK) {
            dbg("D3D11CreateDeviceAndSwapChain got:err = {:08X}!", err);
            return nullptr;
        }

        const auto vtable = *reinterpret_cast<void***>(swapChain);
        const auto present = vtable[IDXGI_PRESENT_FUNCTION_INDEX];

        swapChain->Release();
        device->Release();

        return present;
    }
}
