//
// Created by sexey on 26.10.2025.
//

#ifndef LUA_SCRIPT_ENGINE_H
#define LUA_SCRIPT_ENGINE_H

#include <list>

#include "cs_plain_guarded.h"
#include "binding/lua_binding_interface.h"
#include "binding/lua_guarded_state_interface.h"
#include "gui/gui_widget_regedit.h"

namespace lua
{
    class C_LuaScriptEngine : public binding::C_ILuaGuardedState, public std::enable_shared_from_this<C_LuaScriptEngine>
    {
    private:
        binding::lua_guarded_state_t luaState_;
        std::list<std::unique_ptr<binding::C_ILuaBinding>> bindings_;

        static int ExceptionHandler(lua_State* L, sol::optional<const std::exception&> maybe_exception, sol::string_view description);

        static void PrintOverride(sol::this_state state, sol::variadic_args args);

    public:

        void addBinding(std::unique_ptr<binding::C_ILuaBinding> bind);

        bool initialize();

        binding::lua_guarded_handle_t getLuaState() override { return this->luaState_.lock(); }

        C_LuaScriptEngine() = default;

        ~C_LuaScriptEngine() override = default;
    };
} // lua

#endif //LUA_SCRIPT_ENGINE_H