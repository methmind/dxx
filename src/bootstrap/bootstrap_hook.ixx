//
// Created by sexey on 14.02.2026.
//
module;
#include <memory>

#include "debug/debug_output.h"
#include "MinHook.h"

export module bootstrap.hook;

import service.container;
import service.locator;
import hook.container;
import hook.dispatcher;
import hook.impl.present;
import hook.impl.resizebuffers;

import dx;

namespace bootstrap
{
    bool SetupRendererHook()
    {
        uint8_t* presentFunction = nullptr, *resizeBuffersFunction = nullptr;
        if (!dx::GetSwapChainInfo(reinterpret_cast<void**>(&presentFunction), reinterpret_cast<void**>(&resizeBuffersFunction))) {
            dbg("dx::GetSwapChainInfo got:err = Unable to get IDXGISwapChain::Present function!");
            return false;
        }

        if (const auto err = hook::C_HookContainer::Create(presentFunction, reinterpret_cast<void*>(hook::hkPresent)); err != MH_OK) {
            dbg("Unable to create hook for IDXGISwapChain::Present! err = {}", err);
            return false;
        }

        if (const auto err = hook::C_HookContainer::Create(resizeBuffersFunction, reinterpret_cast<void*>(hook::hkResizeBuffers)); err != MH_OK) {
            dbg("Unable to create hook for IDXGISwapChain::ResizeBuffers! err = {}", err);
            return false;
        }

        return true;
    }

    export bool InitializeHooks(const std::unique_ptr<C_ServiceContainer>& services)
    {
        C_ServiceLocator::Register<hook::C_HookDispatcher>(services->add<hook::C_HookDispatcher>().get());

        auto hooks = services->add<hook::C_HookContainer>();
        if (!hooks->initialize()) {
            dbg("hook::C_HookContainer::initialize got:err = Unable to initialize hooks!");
            return false;
        }

        if (!SetupRendererHook()) {
            dbg("SetupRendererHook got:err = Unable to setup renderer hooks!");
            return false;
        }

        if (!hooks->enable()) {
            dbg("hook::C_HookContainer::enable got:err = Unable to enable hooks!");
            return false;
        }

        return true;
    }
}
