//
// Created by sexey on 26.10.2025.
//

#ifndef LUA_SCRIPT_ENGINE_H
#define LUA_SCRIPT_ENGINE_H

#include <mutex>
#include <shared_mutex>

#include "binding/lua_binding_menu.h"
#include "lua_container_interface.h"
#include "lua_syncer_interface.h"
#include "gui/gui_widget_regedit.h"

namespace lua
{
    class C_LuaScriptEngine : public C_ILuaSyncerInterface, public std::enable_shared_from_this<C_LuaScriptEngine>
    {
    private:
        sol::state luaState_;
        lua_sync_primitive_t luaStateMutex_;

        std::shared_ptr<gui::C_WidgetRegedit> widgetRegedit_;
        std::weak_ptr<C_ILuaContainer> luaContainer_;

        bool applyBindings();

        static int ExceptionHandler(lua_State* L, sol::optional<const std::exception&> maybe_exception, sol::string_view description);

        void printOverride(sol::variadic_args args) const;

    public:

        lua_state_locker_t luaStateLocker() override { return lua_state_locker_t(this->luaStateMutex_); }

        bool initialize(const std::shared_ptr<gui::C_WidgetRegedit>& widgetRegedit,
            const std::weak_ptr<C_ILuaContainer>& luaContainer);

        sol::state& getLuaState() { return this->luaState_; }

        C_LuaScriptEngine() = default;

        ~C_LuaScriptEngine() = default;
    };
} // lua

#endif //LUA_SCRIPT_ENGINE_H