//
// Created by sexey on 07.11.2025.
//

#include "hook_manager.h"

#include "hook_original_invoker.h"
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
#include "sdk/protobuf/gen/dota_usercmd.pb.h"
#include "sdk/singleton/sdk_dota_view_render.h"
#include "sdk/singleton/sdk_game_entity_system.h"
#include "sdk/singleton/sdk_source2_client.h"
#include "sdk/singleton/sdk_source2_engine_to_client.h"
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

    struct CUserCmd
    {
        void* vtable;
        int32_t sequenceNumber;
        dota::CDota2UserCmdPB cmd;
        dota::CMsgVector* crosshair;
    };

    //40 53 48 83 EC ? 8B DA E8 ? ? ? ? 4C 8B C0 - get_user_cmd
    //48 89 4C 24 ? 41 56 41 57 - get sequence number

    using get_user_cmd_fn = CUserCmd * (__thiscall*)(void* entity, int32_t sequenceNumber);
    using get_cmd_sequence_number_fn = void*(__thiscall*)(void* circularBuffer, int32_t playerID);

    //33 C0 83 F9 ? 0F 44 C8
    using get_xz_chto_fn = void*(__fastcall*)(int32_t i);

    //48 83 EC ? 4C 8B 0D ? ? ? ? 4C 8B DA
    using get_xz_chto_fn2 = void*(__fastcall*)(void* xz_chto_fn_ret, int32_t* out);

    get_user_cmd_fn get_user_cmd;
    get_cmd_sequence_number_fn get_cmd_sequence_number;

    get_xz_chto_fn get_xz_chto;
    get_xz_chto_fn2 get_xz_chto2;

    void on_create_move(void* cinput, int32_t slot, bool isActive)
    {
        MH_CALL_ORIGINAL(on_create_move)(cinput, slot, isActive);

        auto unk = get_xz_chto(0);
        if (!unk) {
            return;
        }

        int32_t unk2 = 0;
        get_xz_chto2(unk, &unk2);

        auto unk3 = unk2 - 1;
        if (unk2 == -1) {
            unk3 = -1;
        }

        auto xx = (uint8_t*)GetModuleHandleA("client.dll") + 0x50A5BF8;
        void* global_input_ptr = *(void**)xx;

        auto unk4 = get_cmd_sequence_number(global_input_ptr, unk3);
        int32_t st = *(int32_t*)((uint8_t*)unk4 + 0x5460);

        CUserCmd* userCmd = get_user_cmd(unk, st);
        if (!userCmd) {
            return;
        }

        auto cc = &userCmd->cmd;

        return;
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

        //48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B 01 8B F2 48 8D 54 24
        auto onVerifyMove = memory::FindPattern(clientModule, "85 D2 0F 85 ? ? ? ? 48 8B C4 44 88 40");
        get_user_cmd = reinterpret_cast<get_user_cmd_fn>(memory::FindPattern(clientModule, "40 53 48 83 EC ? 8B DA E8 ? ? ? ? 4C 8B C0"));
        get_cmd_sequence_number = reinterpret_cast<get_cmd_sequence_number_fn>(memory::FindPattern(clientModule, "48 89 4C 24 ? 41 56 41 57"));
        get_xz_chto = reinterpret_cast<get_xz_chto_fn>(memory::FindPattern(clientModule, "33 C0 83 F9 ? 0F 44 C8"));
        get_xz_chto2 = reinterpret_cast<get_xz_chto_fn2>(memory::FindPattern(clientModule, "48 83 EC ? 4C 8B 0D ? ? ? ? 4C 8B DA"));

        if (auto err = MH_CreateHook(onVerifyMove, reinterpret_cast<void*>(on_create_move), nullptr); err != MH_OK) {
            dbg("Unable to create hook for CDOTAInput::onVerifyMove! err = %d", err);
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