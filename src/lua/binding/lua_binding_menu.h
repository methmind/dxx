//
// Created by sexey on 28.10.2025.
//

#ifndef LUA_BINDING_MENU_H
#define LUA_BINDING_MENU_H

#include "lua_container_interface.h"
#include "gui/gui_widget_regedit.h"
#include "sol/sol.hpp"

namespace lua::binding
{
    constexpr auto MENU_NAMESPACE_NAME = "menu";

    bool RegisterMenuApi(sol::state& state,
        const std::shared_ptr<gui::C_WidgetRegedit>& widgetRegedit,
        const std::shared_ptr<C_ILuaContainer>& luaContainer
    );
} // lua

#endif //LUA_BINDING_MENU_H