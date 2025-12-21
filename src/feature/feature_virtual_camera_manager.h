//
// Created by sexey on 10.12.2025.
//

#ifndef DXX_DLC_FEATURE_VIRTUAL_CAMERA_MANAGER_H
#define DXX_DLC_FEATURE_VIRTUAL_CAMERA_MANAGER_H

#include "feature_virtual_camera.h"
#include "renderer/renderer.h"
#include "sdk/custom/sdk_matrices_system.h"
#include "sdk/datatype/sdk_user_cmd.h"

namespace feature
{
    constexpr auto EDGE_PAN_THRESHOLD = 50.0f;

    constexpr auto EDGE_PAN_EXTENDED_THRESHOLD = 300.0f;

    class C_FeatureVirtualCameraManager
    {
    private:
        bool isInitialized_;
        C_FeatureVirtualCamera serverCamera_;

        bool initializeCamera();

        void onLevelInit() { this->isInitialized_ = false; }

        bool getCameraPixelDelta(const DirectX::SimpleMath::Vector3& worldPosition, DirectX::SimpleMath::Vector2& output) const;

        bool getCameraWorldDelta(const DirectX::SimpleMath::Vector3& worldPosition, DirectX::SimpleMath::Vector2& output) const;

        void edgePanThink(const DirectX::SimpleMath::Vector3& crosshairTrace);

        void onCreateMove(sdk::datatype::user_cmd_s* userCmd);

    public:

        bool initialize();

        explicit C_FeatureVirtualCameraManager(const std::shared_ptr<sdk::custom::C_MatricesSystem>& matricesSystem) :
            isInitialized_(false), serverCamera_(matricesSystem) { }

        ~C_FeatureVirtualCameraManager() = default;
    };
} // feature

#endif //DXX_DLC_FEATURE_VIRTUAL_CAMERA_MANAGER_H