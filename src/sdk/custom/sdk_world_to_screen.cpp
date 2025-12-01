//
// Created by sexey on 01.12.2025.
//

#include "sdk_world_to_screen.h"

#include "sdk/math/sdk_math_vector2.h"
#include "sdk/singleton/sdk_render_game_system.h"
#include "service_locator/service_locator.h"

namespace sdk::custom
{
    bool WorldToScreen(const math::Vector2& screenSize, const math::Vector3& in, math::Vector3& out)
    {
        // if return value is 1, so its out of screen...
        if (C_ServiceLocator::getInstance<singleton::C_RenderGameSystem>()->screenTransform(in, out) != 0) {
            return false;
        }

        out.x = ((out.x + 1.0f) * 0.5f) * screenSize.x;
        out.y = screenSize.y - (((out.y + 1.0f) * 0.5f) * screenSize.y);

        return true;
    }
} // sdk