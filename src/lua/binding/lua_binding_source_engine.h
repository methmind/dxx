//
// Created by sexey on 03.12.2025.
//

#ifndef DXX_DLC_LUA_BINDING_SOURCE_ENGINE_H
#define DXX_DLC_LUA_BINDING_SOURCE_ENGINE_H

#include "lua_binding_interface.h"

namespace lua::binding
{
    constexpr auto ENGINE_NAMESPACE_NAME = "engine";

    class C_LuaBindingSourceEngine final : public C_ILuaBinding
    {
    public:

        bool apply(const std::weak_ptr<C_ILuaGuardedState>& guardedState) override;

        C_LuaBindingSourceEngine() = default;

        ~C_LuaBindingSourceEngine() override = default;
    };
} // lua

#endif //DXX_DLC_LUA_BINDING_SOURCE_ENGINE_H
