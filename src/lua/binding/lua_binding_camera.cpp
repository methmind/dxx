//
// Created by sexey on 03.01.2026.
//

#include "lua_binding_camera.h"

#include "debug/debug_output.h"
#include "sdk/singleton/sdk_dota_camera_manager.h"
#include "service_locator/service_locator.h"

namespace lua::binding
{
    bool C_LuaBindingCamera::apply(const std::weak_ptr<C_ILuaGuardedState>& guardedState)
    {
        const auto tmp = guardedState.lock()->getLuaState();
        const auto& luaState = *tmp;

        auto cameraNamespace = luaState->create_named_table(CAMERA_NAMESPACE_NAME);
        if (!cameraNamespace.valid()) {
            dbg("Unable to create renderer namespace!");
            return false;
        }

        cameraNamespace.set_function("set_camera_distance", [](const float newDistance){
            const auto camera = C_ServiceLocator::getInstance<sdk::singleton::C_DotaCamaraManager>()->getPrimaryCamera();
            if (!camera) {
                return;
            }

            camera->setCameraDistance(newDistance);
        });

        return true;
    }
} // lua