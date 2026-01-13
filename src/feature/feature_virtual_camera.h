//
// Created by sexey on 10.12.2025.
//

#ifndef DXX_DLC_FEATURE_VIRTUAL_CAMERA_H
#define DXX_DLC_FEATURE_VIRTUAL_CAMERA_H

#include "sdk/datatype/sdk_user_cmd.h"
#include "sdk/math/sdk_math_vector3.h"

namespace feature
{
    class C_FeatureCameraEmulator
    {
    private:
        sdk::math::vector3 m_vecVirtualCameraOrigin;
        bool m_bIsInitialized = false;

        // Константы
        const float CAMERA_SPEED = 20.0f; // Скорость скролла (можно менять)
        const float EDGE_THRESHOLD = 0.05f; // 5% от края экрана
        const float VIRTUAL_Z = 1200.0f;

        void UpdateEdgePan(sdk::datatype::user_cmd_s* cmd, const sdk::math::vector3& cursorWorld);

        void onCreateMove(sdk::datatype::user_cmd_s* userCmd);

    public:

        bool initialize();

        C_FeatureCameraEmulator() = default;

        ~C_FeatureCameraEmulator() = default;
    };
} // feature

#endif //DXX_DLC_FEATURE_VIRTUAL_CAMERA_H