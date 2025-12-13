//
// Created by sexey on 10.12.2025.
//

#include "feature_virtual_camera.h"

#include "hook/hook_dispatcher.h"
#include "hook/impl/hook_impl_type.h"
#include "memory/pattern_scanner.h"
#include "sdk/sdk_signature.h"
#include "sdk/math/sdk_math_vector3.h"
#include "sdk/singleton/sdk_base_game_system_factory.h"
#include "sdk/singleton/sdk_render_game_system.h"
#include "service_locator/service_locator.h"

namespace feature
{
    void C_FeatureCameraEmulator::onCreateMove(sdk::datatype::user_cmd_s* userCmd)
    {
        auto pbCmd = &userCmd->cmd;
    }

    bool C_FeatureCameraEmulator::initialize()
    {
        C_ServiceLocator::getInstance<hook::C_HookDispatcher>()->subscribe<sdk::datatype::user_cmd_s*>(
            static_cast<hook::hook_id_t>(hook::impl::hook_impl_type_e::CREATE_MOVE),
            [this](sdk::datatype::user_cmd_s* cmd){
                onCreateMove(cmd);
            }
        );

        return true;
    }
} // feature