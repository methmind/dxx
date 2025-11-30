//
// Created by sexey on 30.11.2025.
//

#ifndef DXX_DLC_LUA_BINDING_IMGUI_H
#define DXX_DLC_LUA_BINDING_IMGUI_H

#include "lua_binding_interface.h"

namespace lua::binding
{
    constexpr auto IMGUI_NAMESPACE_NAME = "imgui";

    class C_LuaBindingImgui final : public C_ILuaBinding
    {
    public:

        bool apply(const std::weak_ptr<C_ILuaGuardedState>& guardedState) override;

        C_LuaBindingImgui() = default;

        ~C_LuaBindingImgui() override = default;
    };
} // lua

#endif //DXX_DLC_LUA_BINDING_IMGUI_H
