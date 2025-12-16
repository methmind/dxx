//
// Created by sexey on 15.12.2025.
//

#ifndef DXX_DLC_SDK_DOTA_CAMERA_MANAGER_H
#define DXX_DLC_SDK_DOTA_CAMERA_MANAGER_H

#include "sdk/datatype/sdk_dota_camera.h"

namespace sdk::singleton
{
    /*
     * 48 8B 41 ? C3 CC CC CC CC CC CC CC CC CC CC CC 40 55 48 81 EC
     * sdk::datatype::C_DotaCamera*(__fastcall*)(void* instance);
     */
    constexpr auto GET_CAMERA_INSTANCE_VMT_INDEX = 35;

    class C_DotaCamaraManager
    {
    private:
        void* instance_;

    public:

        datatype::C_DotaCamera* getPrimaryCamera() const;

        bool initialize();

        C_DotaCamaraManager() = default;

        ~C_DotaCamaraManager() = default;
    };
} // sdk

#endif //DXX_DLC_SDK_DOTA_CAMERA_MANAGER_H