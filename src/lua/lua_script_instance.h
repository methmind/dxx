//
// Created by sexey on 26.10.2025.
//

#ifndef LUA_SCRIPT_INSTANCE_H
#define LUA_SCRIPT_INSTANCE_H

#include <list>

#include "gui/gui_widget_interface.h"
#include "gui/gui_widget_regedit.h"
#include "sol/sol.hpp"

namespace lua
{
    constexpr auto LUA_ENTRY_FUNCTION_NAME = "lua_entry";

    constexpr std::string ON_LUA_DISPOSE_HOOK_SID  = "on_lua_dispose";

    class C_LuaScriptInstance
    {
    private:
        sol::protected_function entryPoint_;

        std::list<gui::widget_ptr_t> widgets_;
        std::mutex widgetsMutex_;

        std::shared_ptr<gui::C_WidgetRegedit> widgetRegedit_;

    public:

        void addWidget(const gui::widget_ptr_t& widget);

        bool initialize(const sol::protected_function_result& instance);

        explicit C_LuaScriptInstance(const std::shared_ptr<gui::C_WidgetRegedit>& widgetRegedit) :
            widgetRegedit_(widgetRegedit) {}

        ~C_LuaScriptInstance();
    };
} // lua

#endif //LUA_SCRIPT_INSTANCE_H