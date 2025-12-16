//
// Created by sexey on 15.12.2025.
//

#ifndef DXX_DLC_SDK_DOTA_CAMERA_H
#define DXX_DLC_SDK_DOTA_CAMERA_H

#include "memory/offset.h"
#include "sdk/math/sdk_math_vector2.h"
#include "sdk/math/sdk_math_vector3.h"

namespace sdk::datatype
{
    class C_DotaCamera
    {
    private:
        using camera_set_look_at_pos_t = void(__fastcall*)(C_DotaCamera* instance, float x, float y, bool forceApply, bool isCommitNeed, bool ignorePreviousPos);

        uint8_t size_[0x380]{}; //@note Got it via MemAlloc_GetSizeFunc

    public:

        OFFSET(math::vector3, getCameraPosition, 0x2EC);

        void setLookAtPos(const math::vector2& pos);
    };
}

#endif //DXX_DLC_SDK_DOTA_CAMERA_H