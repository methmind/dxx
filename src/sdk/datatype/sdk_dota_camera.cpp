//
// Created by sexey on 15.12.2025.
//

#include "sdk_dota_camera.h"

#include "memory/pattern_scanner.h"
#include "sdk/sdk_signature.h"

namespace sdk::datatype
{
    void C_DotaCamera::setLookAtPos(const math::vector2& pos)
    {
        static auto fn{reinterpret_cast<camera_set_look_at_pos_t>(memory::FindPattern(GetModuleHandleA("client.dll"), signature::SET_CAMERA_LOOK_AT_POS_FUNC))};
        assert(fn && "Unable to find C_DotaCamera::SetCameraLookAtPos method!");

        fn(this, pos.x, pos.y, true, true, false);
    }
}
