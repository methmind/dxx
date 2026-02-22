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
import hook.impl.frame_stage_notify;

import dx;

import sdk.engine.source2_client;

namespace bootstrap
{
    bool SetupRendererHook()
    {
        const auto presentFunction = dx::GetPresentFunction();
        if (!presentFunction) {
            dbg("dx::GetPresentFunction got:err = Unable to get IDXGISwapChain::Present function!");
            return false;
        }

        if (const auto err = hook::C_HookContainer::Create(presentFunction, reinterpret_cast<void*>(hook::hkPresent)); err != MH_OK) {
            dbg("Unable to create hook for IDXGISwapChain::Present! err = {}", err);
            return false;
        }

        return true;
    }

    bool SetupEventHook()
    {
        const auto source2Client = C_ServiceLocator::Get<sdk::C_Source2Client>();
        const auto frameStageNotifyFunc = source2Client->getFrameStageNotify();
        if (!frameStageNotifyFunc) {
            dbg("Unable to get C_Source2Client::FrameStageNotify function pointer!");
            return false;
        }

        if (const auto err = hook::C_HookContainer::Create(frameStageNotifyFunc, reinterpret_cast<void*>(hook::hkFrameStageNotify)); err != MH_OK) {
            dbg("Unable to create hook for C_Source2Client::FrameStageNotify! err = {}", err);
            return false;
        }

        return true;
    }

    export bool InitializeHooks(const std::unique_ptr<C_ServiceContainer>& services)
    {
        C_ServiceLocator::Register<hook::C_HookDispatcher>(services->add<hook::C_HookDispatcher>().get());

        const auto hooks = services->add<hook::C_HookContainer>();
        if (!hooks->initialize()) {
            dbg("hook::C_HookContainer::initialize got:err = Unable to initialize hooks!");
            return false;
        }

        if (!SetupRendererHook()) {
            dbg("SetupRendererHook got:err = Unable to setup renderer hooks!");
            return false;
        }

        if (!SetupEventHook()) {
            dbg("SetupEventHook got:err = Unable to setup event hooks!");
            return false;
        }

        if (!hooks->enable()) {
            dbg("hook::C_HookContainer::enable got:err = Unable to enable hooks!");
            return false;
        }

        return true;
    }
}
