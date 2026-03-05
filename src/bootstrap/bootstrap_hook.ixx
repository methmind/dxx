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
import hook.impl.get_matrices_for_view;
import hook.impl.level_state_change;
import hook.impl.entity_system;

import dx;

import sdk.engine.source2_client;
import sdk.game_entity_system;

import sdk.locator.matrices_for_view;
import sdk.locator.world_state;

namespace bootstrap
{
    bool SetupRendererHooks()
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

        const auto getMatricesForViewFunc = sdk::GetMatricesForView();
        if (!getMatricesForViewFunc) {
            dbg("Unable to get GetMatricesForView function pointer!");
            return false;
        }

        if (const auto err = hook::C_HookContainer::Create(getMatricesForViewFunc, reinterpret_cast<void*>(hook::hkGetMatricesForView)); err != MH_OK) {
            dbg("Unable to create hook for GetMatricesForView! err = {}", err);
            return false;
        }

        return true;
    }

    bool SetupEventHooks()
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

    bool SetupWorldStateHooks()
    {
        const auto onInit = sdk::GetLevelInitCallback();
        if (!onInit) {
            dbg("Unable to get level init callback function pointer!");
            return false;
        }

        const auto onShutdown = sdk::GetLevelShutdownCallback();
        if (!onShutdown) {
            dbg("Unable to get level init callback function pointer!");
            return false;
        }

        if (const auto err = hook::C_HookContainer::Create(onInit, reinterpret_cast<void*>(hook::hkOnLevelInit)); err != MH_OK) {
            dbg("Unable to create hook for level init callback! err = {}", err);
            return false;
        }

        if (const auto err = hook::C_HookContainer::Create(onShutdown, reinterpret_cast<void*>(hook::hkOnLevelShutdown)); err != MH_OK) {
            dbg("Unable to create hook for level shutdown callback! err = {}", err);
            return false;
        }

        return true;
    }

    bool SetupEntityHooks()
    {
        auto gameEntitySystem = C_ServiceLocator::Get<sdk::C_GameEntitySystem>();
        if (const auto err = hook::C_HookContainer::Create(gameEntitySystem->getOnAddEntityFunc(), reinterpret_cast<void*>(hook::hkOnAddEntity)); err != MH_OK) {
            dbg("Unable to create hook for C_GameEntitySystem::OnAddEntity! err = {}", err);
            return false;
        }

        if (const auto err = hook::C_HookContainer::Create(gameEntitySystem->getOnRemoveEntityFunc(), reinterpret_cast<void*>(hook::hkOnRemoveEntity)); err != MH_OK) {
            dbg("Unable to create hook for C_GameEntitySystem::OnRemoveEntity! err = {}", err);
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

        return true;
    }

    export bool SetupHooks(const std::unique_ptr<C_ServiceContainer>& services)
    {
        const auto hooks = services->get<hook::C_HookContainer>();
        if (!SetupRendererHooks()) {
            dbg("SetupRendererHooks got:err = Unable to setup renderer hooks!");
            return false;
        }

        if (!SetupEventHooks()) {
            dbg("SetupEventHooks got:err = Unable to setup event hooks!");
            return false;
        }

        if (!SetupWorldStateHooks()) {
            dbg("SetupWorldStateHooks got:err = Unable to setup world state hooks!");
            return false;
        }

        if (!SetupEntityHooks()) {
            dbg("SetupEntityHooks got:err = Unable to setup entity hooks!");
            return false;
        }

        if (!hooks->enable()) {
            dbg("hook::C_HookContainer::enable got:err = Unable to enable hooks!");
            return false;
        }

        return true;
    }
}
