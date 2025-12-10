//
// Created by sexey on 06.12.2025.
//

#ifndef DXX_DLC_LUA_BINDING_SDK_MODIFIER_H
#define DXX_DLC_LUA_BINDING_SDK_MODIFIER_H

#include "lua_binding_interface.h"

namespace lua::binding
{
    class C_LuaBindingSdkModifier final : public C_ILuaBinding
    {
    public:

        bool apply(const std::weak_ptr<C_ILuaGuardedState>& guardedState) override;

        C_LuaBindingSdkModifier() = default;

        ~C_LuaBindingSdkModifier() override = default;
    };
} // lua

#endif //DXX_DLC_LUA_BINDING_SDK_MODIFIER_H
