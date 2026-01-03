//
// Created by sexey on 03.01.2026.
//

#ifndef DXX_DLC_LUA_BINDING_CAMERA_H
#define DXX_DLC_LUA_BINDING_CAMERA_H

#include "lua_binding_interface.h"

namespace lua::binding
{
    constexpr auto CAMERA_NAMESPACE_NAME = "camera";

    class C_LuaBindingCamera final : public C_ILuaBinding
    {
    public:

        bool apply(const std::weak_ptr<C_ILuaGuardedState>& guardedState) override;

        C_LuaBindingCamera() = default;

        ~C_LuaBindingCamera() override = default;
    };
} // lua

#endif //DXX_DLC_LUA_BINDING_CAMERA_H