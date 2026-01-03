//
// Created by sexey on 01.12.2025.
//

#include "lua_binding_sdk_math.h"

#include "sdk/math/sdk_math_vector3.h"

namespace lua::binding
{
    bool C_LuaBindingSdkMath::apply(const std::weak_ptr<C_ILuaGuardedState>& guardedState)
    {
        const auto tmp = guardedState.lock()->getLuaState();
        auto& luaState = *tmp;

        luaState->new_usertype<sdk::math::vector3>(
            "vector3",
            sol::constructors<sdk::math::vector3(), sdk::math::vector3(float), sdk::math::vector3(float, float, float)>(),
            "x", &sdk::math::vector3::x,
            "y", &sdk::math::vector3::y,
            "z", &sdk::math::vector3::z
        );

        return true;
    }
} // lua
