//
// Created by sexey on 10.12.2025.
//

#include "application_builder.h"
#include "dx/dx_present.h"
#include "hook/hook_dispatcher.h"
#include "hook/hook_manager.h"
#include "hook/impl/hook_impl_create_move.h"
#include "hook/impl/hook_impl_frame_stage_notify.h"
#include "hook/impl/hook_impl_on_entity_list_change.h"
#include "hook/impl/hook_impl_on_level_state_change.h"
#include "hook/impl/hook_impl_on_render_start.h"
#include "hook/impl/hook_impl_present.h"
#include "memory/pattern_scanner.h"
#include "sdk/sdk_signature.h"
#include "sdk/singleton/sdk_dota_input.h"
#include "sdk/singleton/sdk_dota_view_render.h"
#include "sdk/singleton/sdk_game_entity_system.h"
#include "sdk/singleton/sdk_source2_client.h"
#include "service_locator/service_locator.h"

REGISTER_GLOBAL_SERVICE(hook::C_HookDispatcher);

namespace bootstrap
{
    bool SetupRendererHook()
    {
        const auto presentFunction = dx::GetPresentFunction();
        if (!presentFunction) {
            dbg("Unable to get IDXGISwapChain::Present function!");
            return false;
        }

        if (const auto err = hook::C_HookManager::Create(reinterpret_cast<void*>(presentFunction),
            reinterpret_cast<void*>(hook::impl::hkPresent)); err != MH_OK) {
            dbg("Unable to create hook for IDXGISwapChain::Present! err = %d", err);
            return false;
        }

        const auto onRenderStart = C_ServiceLocator::getInstance<sdk::singleton::C_DotaViewRender>()->onRenderStart();
        if (const auto err = hook::C_HookManager::Create(reinterpret_cast<void*>(onRenderStart),
            reinterpret_cast<void*>(hook::impl::hkOnRenderStart)); err != MH_OK) {
            dbg("Unable to create hook for C_DotaViewRender::OnRenderStart! err = %d", err);
            return false;
        }

        return true;
    }

    bool SetupWorldStateHook(HMODULE clientModule)
    {
        const auto onLevelInit = memory::FindPattern(clientModule, sdk::signature::ON_LEVEL_INIT_CALLBACK);
        if (!onLevelInit) {
            dbg("Unable to find on_level_init callback!");
            return false;
        }

        if (const auto err = hook::C_HookManager::Create(onLevelInit,
            reinterpret_cast<void*>(hook::impl::hkOnLevelInit)); err != MH_OK) {
            dbg("Unable to create hook for ClientModeShared::LevelInit! err = %d", err);
            return false;
        }

        const auto onLevelShutdown = memory::FindPattern(clientModule, sdk::signature::ON_LEVEL_SHUTDOWN_CALLBACK);
        if (!onLevelShutdown) {
            dbg("Unable to find on_level_shutdown callback!");
            return false;
        }

        if (const auto err = hook::C_HookManager::Create(onLevelShutdown,
            reinterpret_cast<void*>(hook::impl::hkOnLevelShutdown)); err != MH_OK) {
            dbg("Unable to create hook for ClientModeShared::LevelShutdown! err = %d", err);
            return false;
        }

        return true;
    }

    bool SetupEntitySystemHook()
    {
        const auto entitySystem = C_ServiceLocator::getInstance<sdk::singleton::C_GameEntitySystem>();
        if (const auto err = hook::C_HookManager::Create(entitySystem->getOnAddEntityFunc(),
            reinterpret_cast<void*>(hook::impl::hkOnAddEntity)); err != MH_OK) {
            dbg("Unable to create hook for C_GameEntitySystem::OnAddEntity! err = %d", err);
            return false;
        }

        if (const auto err = hook::C_HookManager::Create(entitySystem->getOnRemoveEntityFunc(),
            reinterpret_cast<void*>(hook::impl::hkOnRemoveEntity)); err != MH_OK) {
            dbg("Unable to create hook for C_GameEntitySystem::OnRemoveEntity! err = %d", err);
            return false;
        }

        return true;
    }

    bool SetupFrameStageHook()
    {
        const auto fsnFunc = C_ServiceLocator::getInstance<sdk::singleton::C_Source2Client>()->getFrameStageNotify();
        if (const auto err = hook::C_HookManager::Create(reinterpret_cast<void*>(fsnFunc),
            reinterpret_cast<void*>(hook::impl::hkFrameStageNotify)); err != MH_OK) {
            dbg("Unable to create hook for C_Source2Client::FrameStageNotify! err = %d", err);
            return false;
        }

        const auto createMoveFunc = C_ServiceLocator::getInstance<sdk::singleton::C_DotaInput>()->getCreateMove();
        if (const auto err = hook::C_HookManager::Create(reinterpret_cast<void*>(createMoveFunc),
            reinterpret_cast<void*>(hook::impl::hkCreateMove)); err != MH_OK) {
            dbg("Unable to create hook for C_DotaInput::CreateMove! err = %d", err);
            return false;
        }

        return true;
    }

    bool C_ApplicationBuilder::InitializeHookStuff()
    {
        if (!hook::C_HookManager::Initialize()) {
            dbg("Unable to initialize hook::C_HookManager!");
            return false;
        }

        if (!SetupRendererHook()) {
            dbg("Unable to setup renderer hooks!");
            return false;
        }

        if (!SetupEntitySystemHook()) {
            dbg("Unable to setup entity system hooks!");
            return false;
        }

        if (!SetupWorldStateHook(GetModuleHandleA("client.dll"))) {
            dbg("Unable to setup world state change hooks!");
            return false;
        }

        if (!SetupFrameStageHook()) {
            dbg("Unable to setup frame stage hooks!");
            return false;
        }

        return true;
    }
}
