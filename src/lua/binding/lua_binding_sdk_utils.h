//
// Created by sexey on 07.12.2025.
//

#ifndef DXX_DLC_LUA_BINDING_SDK_UTILS_H
#define DXX_DLC_LUA_BINDING_SDK_UTILS_H

#include "lua_binding_interface.h"

namespace lua::binding
{
    class C_LuaBindingSdkUtils final : public C_ILuaBinding
    {
    public:

        bool apply(const std::weak_ptr<C_ILuaGuardedState>& guardedState) override;

        C_LuaBindingSdkUtils() = default;

        ~C_LuaBindingSdkUtils() override = default;
    };
} // lua

#endif //DXX_DLC_LUA_BINDING_SDK_UTILS_H