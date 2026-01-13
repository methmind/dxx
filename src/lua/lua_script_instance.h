//
// Created by sexey on 26.10.2025.
//

#ifndef LUA_SCRIPT_INSTANCE_H
#define LUA_SCRIPT_INSTANCE_H

#include <list>

#include "cs_plain_guarded.h"
#include "gui/gui_widget_interface.h"
#include "binding/lua_guarded_state_interface.h"
#include "sol/sol.hpp"

namespace lua
{
    constexpr auto LUA_ENTRY_FUNCTION_NAME = "lua_entry";

    class C_LuaScriptInstance
    {
    public:
        using dependency_t = std::shared_ptr<std::any>;

    private:
        sol::protected_function entryPoint_;
        libguarded::plain_guarded<std::list<dependency_t>> dependencies_;
        std::weak_ptr<binding::C_ILuaGuardedState> engine_;

    public:

        void addDependency(const dependency_t& dependency);

        bool initialize(const sol::protected_function_result& instance);

        explicit C_LuaScriptInstance(std::weak_ptr<binding::C_ILuaGuardedState> engine) : engine_(std::move(engine)) {}

        ~C_LuaScriptInstance();
    };
} // lua

#endif //LUA_SCRIPT_INSTANCE_H