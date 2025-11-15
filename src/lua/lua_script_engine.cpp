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
        const auto self = weak_from_this();
        if (!binding::RegisterHookApi(self)) {
            dbg("binding::RegisterHookApi got:err = Unable to apply hooks api!");
            return false;
        }

        if (!binding::RegisterMenuApi(self, this->widgetRegedit_, this->luaContainer_.lock())) {
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

    void C_LuaScriptEngine::PrintOverride(sol::this_state state, sol::variadic_args args)
    {
        sol::state_view lua(state);
        const sol::protected_function tostring = lua["tostring"];
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

            // SAFETY: Инициализация вызывается ТОЛЬКО один раз из одного потока
            // ПЕРЕД тем как другие потоки получат доступ к luaState_.
            // Handle жив на протяжении всей инициализации, поэтому ссылка валидна.
            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Wdangling"
            auto& luaState = *this->luaState_.lock();
            #pragma clang diagnostic pop

            luaState.open_libraries(
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

            luaState.set_exception_handler(ExceptionHandler);
            luaState.set_function("print", [](sol::this_state state, sol::variadic_args args) {
                PrintOverride(state, std::move(args));
            });


            if (!applyBindings()) {
                dbg("Unable to apply api bindings to lua engine!");
                return false;
            }

            if (const auto callbackAPI = luaState.require_script(script::CALLBACK_API_CHUNK_NAME,
                script::CALLBACK_API_SCRIPT); !callbackAPI.valid()) {
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