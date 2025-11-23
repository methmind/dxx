//
// Created by sexey on 07.11.2025.
//

#include "hook_manager.h"

#include "MinHook.h"
#include "debug/debug_output.h"
#include "dx/dx_present.h"
#include "impl/hook_impl_on_render_start.h"
#include "impl/hook_impl_frame_stage_notify.h"
#include "impl/hook_impl_on_entity_list_change.h"
#include "impl/hook_impl_on_level_state_change.h"
#include "impl/hook_impl_present.h"
#include "memory/pattern_scanner.h"
#include "sdk/sdk_signature.h"
#include "sdk/singleton/sdk_dota_view_render.h"
#include "sdk/singleton/sdk_game_entity_system.h"
#include "sdk/singleton/sdk_source2_client.h"
#include "service_locator/service_locator.h"

namespace hook
{
    bool C_HookManager::initializeRender()
    {
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

        const auto onRenderStart = C_ServiceLocator::getInstance<sdk::singleton::C_DotaViewRender>()->onRenderStart();
        if (const auto err = MH_CreateHook(reinterpret_cast<void*>(onRenderStart),
            reinterpret_cast<void*>(impl::hkOnRenderStart), nullptr); err != MH_OK) {
            dbg("Unable to create hook for C_DotaViewRender::OnRenderStart! err = %d", err);
            return false;
        }

        return true;
    }

    bool C_HookManager::initializeWorld(HMODULE clientModule)
    {
        const auto onLevelInit = memory::FindPattern(clientModule, sdk::signature::ON_LEVEL_INIT_CALLBACK);
        if (!onLevelInit) {
            dbg("Unable to find on_level_init callback!");
            return false;
        }

        if (const auto err = MH_CreateHook(onLevelInit,
            reinterpret_cast<void*>(impl::hkOnLevelInit), nullptr); err != MH_OK) {
            dbg("Unable to create hook for ClientModeShared::LevelInit! err = %d", err);
            return false;
        }

        const auto onLevelShutdown = memory::FindPattern(clientModule, sdk::signature::ON_LEVEL_SHUTDOWN_CALLBACK);
        if (!onLevelShutdown) {
            dbg("Unable to find on_level_shutdown callback!");
            return false;
        }

        if (const auto err = MH_CreateHook(onLevelShutdown,
            reinterpret_cast<void*>(impl::hkOnLevelShutdown), nullptr); err != MH_OK) {
            dbg("Unable to create hook for ClientModeShared::LevelShutdown! err = %d", err);
            return false;
        }

        return true;
    }

    bool C_HookManager::initializeEntity(HMODULE clientModule)
    {
        const auto entitySystem = C_ServiceLocator::getInstance<sdk::singleton::C_GameEntitySystem>();
        if (const auto err = MH_CreateHook(entitySystem->getOnAddEntityFunc(),
            reinterpret_cast<void*>(impl::hkOnAddEntity), nullptr); err != MH_OK) {
            dbg("Unable to create hook for C_GameEntitySystem::OnAddEntity! err = %d", err);
            return false;
        }

        if (const auto err = MH_CreateHook(entitySystem->getOnRemoveEntityFunc(),
            reinterpret_cast<void*>(impl::hkOnRemoveEntity), nullptr); err != MH_OK) {
            dbg("Unable to create hook for C_GameEntitySystem::OnRemoveEntity! err = %d", err);
            return false;
        }

        return true;
    }

    bool C_HookManager::initialize()
    {
        if (const auto err = MH_Initialize(); err != MH_OK) {
            dbg("MH_Initialize got:err = %d", err);
            return false;
        }

        if (!initializeRender()) {
            dbg("Unable to initialize renderer hooks!");
            return false;
        }

        const auto fsnFunc = C_ServiceLocator::getInstance<sdk::singleton::C_Source2Client>()->getFrameStageNotify();
        if (const auto err = MH_CreateHook(reinterpret_cast<void*>(fsnFunc),
            reinterpret_cast<void*>(impl::hkFrameStageNotify), nullptr); err != MH_OK) {
            dbg("Unable to create hook for C_Source2Client::FrameStageNotify! err = %d", err);
            return false;
        }

        const auto clientModule = GetModuleHandleA("client.dll");
        if (!initializeWorld(clientModule)) {
            dbg("Unable to initialize world`s state change hooks!");
            return false;
        }

        if (!initializeEntity(clientModule)) {
            dbg("Unable to initialize entity list changes hook!");
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

    void C_HookManager::disable()
    {
        MH_Uninitialize();
    }
} // hook