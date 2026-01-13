//
// Created by sexey on 30.11.2025.
//

#include "lua_binding_imgui.h"

#include "imgui.h"
#include "debug/debug_output.h"

namespace lua::binding
{
    bool C_LuaBindingImgui::apply(const std::weak_ptr<C_ILuaGuardedState>& guardedState)
    {
        const auto tmp = guardedState.lock()->getLuaState();
        auto& luaState = *tmp;

        auto imguiNamespace = luaState[IMGUI_NAMESPACE_NAME].get_or_create<sol::table>();
        if (!imguiNamespace.valid()) {
            dbg("Unable to create imgui namespace!");
            return false;
        }

        luaState.new_usertype<ImVec2>("ImVec2",
            sol::constructors<ImVec2(), ImVec2(float, float)>(),
            "x", &ImVec2::x,
            "y", &ImVec2::y
        );

        luaState.new_usertype<ImVec4>("ImVec4",
            sol::constructors<ImVec4(), ImVec4(float, float, float, float)>(),
            "x", &ImVec4::x,
            "y", &ImVec4::y,
            "z", &ImVec4::z,
            "w", &ImVec4::w
        );

        imguiNamespace.set_function("color32", [](float r, float g, float b, float a) {
            return IM_COL32(r, g, b, a);
        });

        return true;
    }
} // lua