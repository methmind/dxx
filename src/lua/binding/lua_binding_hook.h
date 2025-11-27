//
// Created by sexey on 13.11.2025.
//

#ifndef DXX_DLC_LUA_BINDING_HOOK_H
#define DXX_DLC_LUA_BINDING_HOOK_H

#include "lua_binding_interface.h"
#include "lua_guarded_state_interface.h"

/*
 * @brief Do not use any stuff from this anywhere except callback_api.lua
*/

namespace lua::binding
{
    constexpr auto HOOK_NAMESPACE_NAME = "hook";

    class C_LuaBindingHook final : public C_ILuaBinding
    {
    public:

        bool apply(const std::weak_ptr<C_ILuaGuardedState>& guardedState) override;

        C_LuaBindingHook() = default;

        ~C_LuaBindingHook() override = default;
    };
} // lua

#endif //DXX_DLC_LUA_BINDING_HOOK_H