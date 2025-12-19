//
// Created by sexey on 10.12.2025.
//

#ifndef DXX_DLC_FEATURE_VIRTUAL_CAMERA_MANAGER_H
#define DXX_DLC_FEATURE_VIRTUAL_CAMERA_MANAGER_H

#include "feature_virtual_camera.h"
#include "renderer/renderer.h"
#include "renderer/renderer_math.h"
#include "sdk/custom/sdk_matrices_system.h"
#include "sdk/datatype/sdk_user_cmd.h"

namespace feature
{
    constexpr auto EDGE_PAN_THRESHOLD = 50.0f;

    class C_FeatureVirtualCameraManager
    {
    private:
        bool isInitialized_;
        C_FeatureVirtualCamera serverCamera_;
        std::shared_ptr<sdk::custom::C_MatricesSystem> matricesSystem_;
        std::shared_ptr<render::C_Renderer> render_;

        bool initializeCamera();

        void onLevelInit() { this->isInitialized_ = false; }

        void onCreateMove(sdk::datatype::user_cmd_s* userCmd);

    public:

        bool initialize();

        explicit C_FeatureVirtualCameraManager(const std::shared_ptr<sdk::custom::C_MatricesSystem>& matricesSystem, const std::shared_ptr<render::C_Renderer>& rener) :
            isInitialized_(false), serverCamera_(matricesSystem), matricesSystem_(matricesSystem), render_(rener) { }

        ~C_FeatureVirtualCameraManager() = default;
    };
} // feature

#endif //DXX_DLC_FEATURE_VIRTUAL_CAMERA_MANAGER_H