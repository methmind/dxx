//
// Created by sexey on 26.10.2025.
//

#ifndef LUA_SCRIPT_ENGINE_H
#define LUA_SCRIPT_ENGINE_H

#include "cs_plain_guarded.h"
#include "lua_container_interface.h"
#include "binding/lua_guarded_state_interface.h"
#include "gui/gui_widget_regedit.h"

namespace lua
{
    class C_LuaScriptEngine : public binding::C_ILuaGuardedState, public std::enable_shared_from_this<C_LuaScriptEngine>
    {
    private:
        binding::lua_guarded_state_t luaState_;
        std::shared_ptr<gui::C_WidgetRegedit> widgetRegedit_;
        std::weak_ptr<C_ILuaContainer> luaContainer_;

        bool applyBindings();

        static int ExceptionHandler(lua_State* L, sol::optional<const std::exception&> maybe_exception, sol::string_view description);

        static void PrintOverride(sol::this_state state, sol::variadic_args args);

    public:

        bool initialize(const std::shared_ptr<gui::C_WidgetRegedit>& widgetRegedit,
            const std::weak_ptr<C_ILuaContainer>& luaContainer);

        binding::lua_guarded_handle_t getLuaState() override { return this->luaState_.lock(); }

        C_LuaScriptEngine() = default;

        ~C_LuaScriptEngine() override = default;
    };
} // lua

#endif //LUA_SCRIPT_ENGINE_H