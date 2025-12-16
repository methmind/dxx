//
// Created by sexey on 10.12.2025.
//

#ifndef DXX_DLC_FEATURE_VIRTUAL_CAMERA_H
#define DXX_DLC_FEATURE_VIRTUAL_CAMERA_H

#include <optional>

#include "sdk/datatype/sdk_dota_camera.h"
#include "sdk/datatype/sdk_user_cmd.h"
#include "sdk/math/sdk_math_vector3.h"

namespace feature
{
    constexpr auto EDGE_PAN_THRESHOLD = 0.05f;

    constexpr auto DEFAULT_CAMERA_DISTANCE = 1200.0f;

    class C_FeatureCameraEmulator
    {
    private:
        std::optional<sdk::datatype::C_DotaCamera> dummyCamera_;

        void fixCameraPosition(sdk::datatype::user_cmd_s* userCmd);

        void onLevelInit();

        void onCreateMove(sdk::datatype::user_cmd_s* userCmd);

    public:

        bool initialize();

        C_FeatureCameraEmulator() = default;

        ~C_FeatureCameraEmulator() = default;
    };
} // feature

#endif //DXX_DLC_FEATURE_VIRTUAL_CAMERA_H