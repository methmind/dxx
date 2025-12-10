//
// Created by sexey on 10.12.2025.
//

#include "sdk_input_controller.h"

#include "hook/hook_dispatcher.h"
#include "hook/impl/hook_impl_type.h"
#include "sdk/singleton/sdk_dota_input.h"
#include "sdk/singleton/sdk_game_entity_system.h"
#include "service_locator/service_locator.h"

namespace sdk::custom
{
    void C_InputController::OnCreateMove()
    {
        const auto localController = C_ServiceLocator::getInstance<singleton::C_GameEntitySystem>()->getPlayerController(0);
        if (!localController) {
            return;
        }

        const auto cmd = C_ServiceLocator::getInstance<singleton::C_DotaInput>()->getUserCmd(localController);
        if (!cmd) {
            return;
        }

        C_ServiceLocator::getInstance<hook::C_HookDispatcher>()->invoke<iface::user_cmd_s*>(
            static_cast<hook::hook_id_t>(hook::impl::hook_impl_type_e::CREATE_MOVE),
            cmd
        );
    }

    bool C_InputController::Initialize()
    {
        C_ServiceLocator::getInstance<hook::C_HookDispatcher>()->subscribe(
            static_cast<hook::hook_id_t>(hook::impl::hook_impl_type_e::CREATE_MOVE_RAW), OnCreateMove
        );

        return true;
    }
} // sdk