//
// Created by sexey on 01.12.2025.
//

#ifndef DXX_DLC_SDK_WORLD_TO_SCREEN_H
#define DXX_DLC_SDK_WORLD_TO_SCREEN_H

#include "sdk/math/sdk_math_vector3.h"
#include "sdk/math/sdk_math_vector2.h"

namespace sdk::custom
{
    bool WorldToScreen(const math::Vector2& screenSize, const math::Vector3& in, math::Vector3& out);
} // sdk

#endif //DXX_DLC_SDK_WORLD_TO_SCREEN_H