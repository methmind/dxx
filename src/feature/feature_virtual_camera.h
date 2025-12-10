//
// Created by sexey on 10.12.2025.
//

#ifndef DXX_DLC_FEATURE_VIRTUAL_CAMERA_H
#define DXX_DLC_FEATURE_VIRTUAL_CAMERA_H

#include "sdk/datatype/sdk_user_cmd.h"

namespace feature
{
    class C_FeatureCameraEmulator
    {
    private:

        void onCreateMove(sdk::datatype::user_cmd_s* userCmd);

    public:

        bool initialize();

        C_FeatureCameraEmulator() = default;

        ~C_FeatureCameraEmulator() = default;
    };
} // feature

#endif //DXX_DLC_FEATURE_VIRTUAL_CAMERA_H