//
// Created by sexey on 26.10.2025.
//

#include "lua_script_engine.h"

#include <format>

#include "binding/lua_binding_hook.h"
#include "binding/lua_binding_menu.h"
#include "debug/debug_output.h"
#include "script/callback_api.h"

namespace lua
{
    bool C_LuaScriptEngine::applyBindings()
    {
        const auto lockPtr = this->luaContainer_.lock();
        if (!lockPtr) {
            dbg("Unable to get shared ptr for lua container!");
            return false;
        }

        if (!binding::RegisterHookApi(this->luaState_, weak_from_this())) {
            dbg("binding::RegisterHookApi got:err = Unable to apply hooks api!");
            return false;
        }

        if (!binding::RegisterMenuApi(this->luaState_, this->widgetRegedit_, lockPtr)) {
            dbg("binding::RegisterMenuApi got:err = Unable to apply menu api binding!");
            return false;
        }

        return true;
    }

    int C_LuaScriptEngine::ExceptionHandler(lua_State* L, sol::optional<const std::exception&> maybe_exception,
        sol::string_view description)
    {
        dbg("[Lua]: %s:%s", (maybe_exception) ? maybe_exception->what() : "", description.data());
        return sol::stack::push(L, description);
    }

    void C_LuaScriptEngine::printOverride(sol::variadic_args args) const
    {
        sol::state_view lua_state = this->luaState_;
        const sol::protected_function tostring = lua_state["tostring"];
        std::string output;
        bool first = true;
        for (auto arg : args) {
            sol::object obj = arg;
            sol::protected_function_result result = tostring(obj);
            if (result.valid()) {
                std::string str = result;
                if (!first) {
                    output += "\t";
                }
                output += str;
                first = false;
            } else {
                if (!first) {
                    output += "\t";
                }
                output += "<cannot convert to string>";
                first = false;
            }
        }

        dbg("[Lua]: %s", output.c_str());
    }

    bool C_LuaScriptEngine::initialize(const std::shared_ptr<gui::C_WidgetRegedit>& widgetRegedit,
                                       const std::weak_ptr<C_ILuaContainer>& luaContainer)
    {
        try {
            this->widgetRegedit_ = widgetRegedit;
            this->luaContainer_ = luaContainer;
            this->luaState_.open_libraries(
                sol::lib::base,
                sol::lib::package,
                sol::lib::coroutine,
                sol::lib::string,
                sol::lib::os,
                sol::lib::math,
                sol::lib::table,
                sol::lib::debug,
                sol::lib::bit32,
                sol::lib::io,
                sol::lib::ffi
            );

            this->luaState_.set_exception_handler(ExceptionHandler);
            this->luaState_.set_function("print", [this](sol::variadic_args args) {
                printOverride(std::move(args));
            });

            if (!applyBindings()) {
                dbg("Unable to apply api bindings to lua engine!");
                return false;
            }

            if (const auto callbackAPI = this->luaState_.safe_script(script::CALLBACK_API_SCRIPT);
                !callbackAPI.valid()) {
                dbg("Unable to load callback api script!");
                return false;
            }

            return true;
        } catch (const std::exception& ex) {
            dbg("Critical exception: %s", ex.what());
            return false;
        }
    }
} // lua