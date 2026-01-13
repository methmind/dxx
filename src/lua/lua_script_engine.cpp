//
// Created by sexey on 26.10.2025.
//

#include "lua_script_engine.h"

#include <windows.h>
#include <format>

#include "binding/lua_binding_hook.h"
#include "binding/lua_binding_menu.h"
#include "debug/debug_output.h"

namespace lua
{
    int C_LuaScriptEngine::ExceptionHandler(lua_State* L, sol::optional<const std::exception&> maybe_exception,
        sol::string_view description)
    {
        dbg("[Lua]: %s:%s", (maybe_exception) ? maybe_exception->what() : "", description.data());
        return sol::stack::push(L, description);
    }

    void C_LuaScriptEngine::LuaPanicHandler(sol::optional<std::string> message)
    {
        MessageBoxA(nullptr, (message) ? message.value().c_str() : "Unknown panic!", "LuaVM", 0);
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

    void C_LuaScriptEngine::addBinding(std::unique_ptr<binding::C_ILuaBinding> bind)
    {
        bind->apply(weak_from_this());
        this->bindings_.push_back(std::move(bind));
    }

    bool C_LuaScriptEngine::initialize()
    {
        try {
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
            luaState.set_panic(sol::c_call<decltype(&LuaPanicHandler), &LuaPanicHandler>);
            luaState.set_function("print", [](sol::this_state state, sol::variadic_args args) {
                PrintOverride(state, std::move(args));
            });

            return true;
        } catch (const std::exception& ex) {
            dbg("Critical exception: %s", ex.what());
            return false;
        }
    }
} // lua