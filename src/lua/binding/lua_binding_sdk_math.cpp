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

        auto luaVector3 = luaState.new_usertype<sdk::math::vector3>(
            "vector3",
            sol::constructors<sdk::math::vector3(), sdk::math::vector3(float), sdk::math::vector3(float, float, float)>(),
            "x", &sdk::math::vector3::x,
            "y", &sdk::math::vector3::y,
            "z", &sdk::math::vector3::z
        );

        luaVector3.set_function("is_near", &sdk::math::vector3::is_near);
        luaVector3.set_function("dot", &sdk::math::vector3::dot);
        luaVector3.set_function("cross", &sdk::math::vector3::cross);
        luaVector3.set_function("length_sqr", &sdk::math::vector3::length_sqr);
        luaVector3.set_function("length_2d_sqr", &sdk::math::vector3::length_2d_sqr);
        luaVector3.set_function("length", &sdk::math::vector3::length);
        luaVector3.set_function("length_2d", &sdk::math::vector3::length_2d);
        luaVector3.set_function("length_safe", &sdk::math::vector3::length_safe);
        luaVector3.set_function("length_2d_safe", &sdk::math::vector3::length_2d_safe);
        luaVector3.set_function("dist_to", &sdk::math::vector3::dist_to);
        luaVector3.set_function("dist_to_sqr", &sdk::math::vector3::dist_to_sqr);
        luaVector3.set_function("is_zero", &sdk::math::vector3::is_zero);
        luaVector3.set_function("is_near_zero", &sdk::math::vector3::is_near_zero);
        luaVector3.set_function("is_valid", &sdk::math::vector3::is_valid);
        luaVector3.set_function("normalized", &sdk::math::vector3::normalized);
        luaVector3.set_function("normalize", &sdk::math::vector3::normalize);
        luaVector3.set_function("project_onto", &sdk::math::vector3::project_onto);
        luaVector3.set_function("reflect", &sdk::math::vector3::reflect);
        luaVector3.set_function("clamp", &sdk::math::vector3::clamp);
        luaVector3.set_function("clamped", &sdk::math::vector3::clamped);
        luaVector3.set_function("sanitize_angles", &sdk::math::vector3::sanitize_angles);
        luaVector3.set_function("to_angles", &sdk::math::vector3::to_angles);

        // Статические методы (вызываются как vector3.lerp(...))
        luaVector3.set_function("lerp", &sdk::math::vector3::lerp);
        luaVector3.set_function("min", &sdk::math::vector3::min);
        luaVector3.set_function("max", &sdk::math::vector3::max);
        luaVector3.set_function("from_angles", &sdk::math::vector3::from_angles);

        // Операторы
        luaVector3.set_function(sol::meta_function::addition, sol::overload(
            [](const sdk::math::vector3& a, const sdk::math::vector3& b) { return a + b; },
            [](const sdk::math::vector3& a, float s) { return a + s; },
            [](float s, const sdk::math::vector3& a) { return s + a; }
        ));

        luaVector3.set_function(sol::meta_function::subtraction, sol::overload(
            [](const sdk::math::vector3& a, const sdk::math::vector3& b) { return a - b; },
            [](const sdk::math::vector3& a, float s) { return a - s; },
            [](float s, const sdk::math::vector3& a) { return s - a; }
        ));

        luaVector3.set_function(sol::meta_function::multiplication, sol::overload(
            [](const sdk::math::vector3& a, const sdk::math::vector3& b) { return a * b; },
            [](const sdk::math::vector3& a, float s) { return a * s; },
            [](float s, const sdk::math::vector3& a) { return s * a; }
        ));

        luaVector3.set_function(sol::meta_function::division, sol::overload(
            [](const sdk::math::vector3& a, const sdk::math::vector3& b) { return a / b; },
            [](const sdk::math::vector3& a, float s) { return a / s; }
        ));

        luaVector3.set_function(sol::meta_function::unary_minus, [](const sdk::math::vector3& v) { return -v; });
        luaVector3.set_function(sol::meta_function::equal_to, &sdk::math::vector3::operator==);

        return true;
    }
} // lua
