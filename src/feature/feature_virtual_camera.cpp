//
// Created by sexey on 10.12.2025.
//

#include "feature_virtual_camera.h"

#include "hook/hook_dispatcher.h"
#include "hook/impl/hook_impl_type.h"
#include "service_locator/service_locator.h"

namespace feature
{
    void C_FeatureCameraEmulator::onCreateMove(sdk::datatype::user_cmd_s* userCmd)
    {
        auto pbCmd = &userCmd->cmd;
        dbg("Camera: %d | %d", pbCmd->cameraposition_x(), pbCmd->cameraposition_y());
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