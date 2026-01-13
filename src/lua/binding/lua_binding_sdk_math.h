//
// Created by sexey on 01.12.2025.
//

#ifndef DXX_DLC_LUA_BINDING_SDK_MATH_H
#define DXX_DLC_LUA_BINDING_SDK_MATH_H

#include "lua_binding_interface.h"

namespace lua::binding
{
    class C_LuaBindingSdkMath final : public C_ILuaBinding
    {
    public:

        bool apply(const std::weak_ptr<C_ILuaGuardedState>& guardedState) override;

        C_LuaBindingSdkMath() = default;

        ~C_LuaBindingSdkMath() override = default;
    };
} // lua

#endif //DXX_DLC_LUA_BINDING_SDK_MATH_H