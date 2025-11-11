//
// Created by sexey on 07.11.2025.
//

#include "hook_manager.h"

#include "MinHook.h"
#include "debug/debug_output.h"
#include "dx/dx_present.h"
#include "impl/hook_impl_on_render_start.h"
#include "impl/hook_impl_present.h"
#include "sdk/dota_view_render.h"
#include "service_locator/service_locator.h"

namespace hook
{
    bool C_HookManager::initialize()
    {
        if (const auto err = MH_Initialize(); err != MH_OK) {
            dbg("MH_Initialize got:err = %d", err);
            return false;
        }

        const auto presentFunction = dx::GetPresentFunction();
        if (!presentFunction) {
            dbg("Unable to get IDXGISwapChain::Present function!");
            return false;
        }

        if (const auto err = MH_CreateHook(reinterpret_cast<void*>(presentFunction),
            reinterpret_cast<void*>(impl::hkPresent), nullptr); err != MH_OK) {
            dbg("Unable to create hook for IDXGISwapChain::Present! err = %d", err);
            return false;
        }

        const auto onRenderStart = C_ServiceLocator::getInstance<sdk::C_DotaViewRender>()->onRenderStart();

        if (const auto err = MH_CreateHook(reinterpret_cast<void*>(onRenderStart),
            reinterpret_cast<void*>(impl::hkOnRenderStart), nullptr); err != MH_OK) {
            dbg("Unable to create hook for C_DotaViewRender::OnRenderStart! err = %d", err);
            return false;
        }

        return true;
    }

    bool C_HookManager::enable()
    {
        if (const auto err = MH_EnableHook(MH_ALL_HOOKS); err != MH_OK) {
            dbg("MH_EnableHook got:err = %d", err);
            return false;
        }

        return true;
    }

    C_HookManager::~C_HookManager()
    {
        MH_DisableHook(MH_ALL_HOOKS);
        MH_Uninitialize();
    }
} // hook