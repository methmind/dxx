//
// Created by sexey on 10.12.2025.
//

#include "feature_virtual_camera_manager.h"

#include "imgui_internal.h"
#include "hook/hook_dispatcher.h"
#include "hook/impl/hook_impl_type.h"
#include "memory/pattern_scanner.h"
#include "sdk/math/sdk_math_vector3.h"
#include "sdk/math/sdk_math_quantize_point.h"
#include "service_locator/service_locator.h"
#include "sdk/singleton/sdk_dota_camera_manager.h"
#include "sdk/singleton/sdk_source2_engine_to_client.h"
#include "renderer/renderer_math.h"
#include "sdk/util/sdk_game_state.h"
#include "sdk/custom/sdk_event_game_state.h"

namespace feature
{
    void C_FeatureVirtualCameraManager::onFireEvent(sdk::datatype::C_GameEvent* event)
    {
        if (!sdk::custom::ValidateGameState(event, sdk::util::game_state_e::DOTA_GAMERULES_STATE_STRATEGY_TIME)) {
            return;
        }

        if (!this->serverCamera_.initialize()) {
            assert("Unable to initialize server camera!");
        }
    }

    bool C_FeatureVirtualCameraManager::getCameraPixelDelta(const DirectX::SimpleMath::Vector3& worldPosition, DirectX::SimpleMath::Vector2& output) const
    {
        const auto display = C_ServiceLocator::getInstance<sdk::singleton::C_Source2EngineToClient>()->getScreenSize();
        DirectX::SimpleMath::Vector2 screenPosition{};

        if (!render::WorldToScreen(worldPosition, screenPosition,
            DirectX::SimpleMath::Vector2(display.x, display.y),
            this->serverCamera_.getWorldToProjectionMatrix())) {
            return false;
        }

        //todo Change this `GetAsyncKeyState` meme to something normal...
        const float threshold = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) ? EDGE_PAN_EXTENDED_THRESHOLD : EDGE_PAN_THRESHOLD;
        if (screenPosition.x > display.x - threshold) {
            output.x = screenPosition.x - (display.x - threshold);
        } else if (screenPosition.x < threshold) {
            output.x = screenPosition.x - threshold;
        }

        if (screenPosition.y > display.y - threshold) {
            output.y = screenPosition.y - (display.y - threshold);
        } else if (screenPosition.y < threshold) {
            output.y = screenPosition.y - threshold;
        }

        return true;
    }

    bool C_FeatureVirtualCameraManager::getCameraWorldDelta(const DirectX::SimpleMath::Vector3& worldPosition, DirectX::SimpleMath::Vector2& output) const
    {
        DirectX::SimpleMath::Vector2 pixelDelta{};
        if (!getCameraPixelDelta(worldPosition,pixelDelta)) {
            return false;
        }

        if (pixelDelta.x == 0 && pixelDelta.y == 0) {
            return true;
        }

        const auto& viewMatrix = this->serverCamera_.getWorldToViewMatrix();
        DirectX::SimpleMath::Vector2 xVector(viewMatrix.m[0][0], viewMatrix.m[0][1]);
        DirectX::SimpleMath::Vector2 yVector(viewMatrix.m[1][0], viewMatrix.m[1][1]);

        // 1200 distance ~ 1.2 units/pixel.
        const float scale = this->serverCamera_.getCameraDistance() * 0.001f;

        // Считаем смещение
        output.x = (xVector.x * pixelDelta.x - yVector.x * pixelDelta.y) * scale;
        output.y = (xVector.y * pixelDelta.x - yVector.y * pixelDelta.y) * scale;

        return true;
    }

    void C_FeatureVirtualCameraManager::edgePanThink(const DirectX::SimpleMath::Vector3& crosshairTrace)
    {
        DirectX::SimpleMath::Vector2 worldDelta{};
        auto cameraPosition = this->serverCamera_.getCameraPosition();

        if (!getCameraWorldDelta(crosshairTrace, worldDelta)) {
            worldDelta.x = crosshairTrace.x - cameraPosition.x;
            worldDelta.y = crosshairTrace.y - cameraPosition.y;
        }

        if (worldDelta.x != 0 || worldDelta.y != 0) {
            cameraPosition.x += worldDelta.x;
            cameraPosition.y += worldDelta.y;
            this->serverCamera_.updateCameraPosition(cameraPosition);
        }
    }

    void C_FeatureVirtualCameraManager::onCreateMove(sdk::datatype::user_cmd_s* userCmd)
    {
        auto& cmd = userCmd->cmd;
        if (const auto primaryCamera = C_ServiceLocator::getInstance<sdk::singleton::C_DotaCamaraManager>()->getPrimaryCamera();
            primaryCamera->getCameraDistance() <= DEFAULT_CAMERA_DISTANCE) {
            return;
        }

        if (!this->serverCamera_.isInitialized()) {
            return;
        }

        const auto displaySize = C_ServiceLocator::getInstance<sdk::singleton::C_Source2EngineToClient>()->getScreenSize();
        const DirectX::SimpleMath::Vector3 crosshairTrace = {
            cmd.crosshairtrace().x(),
            cmd.crosshairtrace().y(),
            cmd.crosshairtrace().z()
        };

        edgePanThink(crosshairTrace);

        DirectX::SimpleMath::Vector2 crosshairScreenPosition;
        if (!render::WorldToScreen(crosshairTrace, crosshairScreenPosition,
            reinterpret_cast<const DirectX::SimpleMath::Vector2&>(displaySize),
            this->serverCamera_.getWorldToProjectionMatrix())) [[unlikely]] {
            return;
        }

        const auto quantized = sdk::math::QuantizePoint(displaySize, reinterpret_cast<sdk::math::vector2&>(crosshairScreenPosition));
        const auto cameraLookAt = this->serverCamera_.getLookAt();
        cmd.set_cameraposition_x(cameraLookAt.x);
        cmd.set_cameraposition_y(cameraLookAt.y);
        cmd.mutable_base()->set_mousedx(quantized.x);
        cmd.mutable_base()->set_mousedy(quantized.y);
    }

    bool C_FeatureVirtualCameraManager::initialize()
    {
        const auto hookDispatcher = C_ServiceLocator::getInstance<hook::C_HookDispatcher>();

        hookDispatcher->subscribe<sdk::datatype::user_cmd_s*>(
            static_cast<hook::hook_id_t>(hook::impl::hook_impl_type_e::CREATE_MOVE),
            [this](sdk::datatype::user_cmd_s* cmd){
                onCreateMove(cmd);
            }
        );

        hookDispatcher->subscribe<void*, void*>(
            static_cast<hook::hook_id_t>(hook::impl::hook_impl_type_e::FIRE_EVENT),
            [this](void*, void* event){
                onFireEvent(static_cast<sdk::datatype::C_GameEvent*>(event));
            }
        );

        hookDispatcher->subscribe(
            static_cast<hook::hook_id_t>(hook::impl::hook_impl_type_e::ON_LEVEL_INIT),
            [this] { this->serverCamera_.dispose(); }
        );

        return true;
    }
} // feature