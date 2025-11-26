//
// Created by sexey on 28.10.2025.
//

#ifndef LUA_BINDING_MENU_H
#define LUA_BINDING_MENU_H

#include "lua_binding_interface.h"
#include "lua/lua_container_interface.h"
#include "gui/gui_widget_regedit.h"
#include "lua_guarded_state_interface.h"

namespace lua::binding
{
    constexpr auto MENU_NAMESPACE_NAME = "menu";

    class C_LuaBindingMenu : public C_ILuaBinding
    {
    private:
        std::shared_ptr<gui::C_WidgetRegedit> widgetRegedit_;
        std::shared_ptr<C_ILuaContainer> luaContainer_;

    public:

        bool apply(const std::weak_ptr<C_ILuaGuardedState>& guardedState) override;

        C_LuaBindingMenu(const std::shared_ptr<gui::C_WidgetRegedit>& widgetRegedit,
            const std::shared_ptr<C_ILuaContainer>& luaContainer)
            : widgetRegedit_(widgetRegedit), luaContainer_(luaContainer) {}

        ~C_LuaBindingMenu() override = default;
    };
} // lua

#endif //LUA_BINDING_MENU_H