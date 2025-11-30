//
// Created by sexey on 13.11.2025.
//

#include "lua_binding_hook.h"

#include <format>

#include "debug/debug_output.h"
#include "hook/hook_dispatcher.h"
#include "hook/impl/hook_impl_type.h"
#include "lua/script/callback_api.h"
#include "service_locator/service_container.h"
#include "service_locator/service_locator.h"

namespace lua::binding
{
    /* todo Возможно, что такая реализация была бы лучше... С ней можно убрать мануальную очистку C_HookDispatcher
    // Вспомогательная функция для проталкивания аргументов
    template<typename ... arg_t>
    __forceinline void push_args_to_stack(lua_State* L, arg_t ... args) {
        (..., sol::stack::push(L, args));
    }

    template<typename ... arg_t>
    void RegisterHookCallback(sol::table& hookNamespace, const std::string_view& name, hook::hook_id_t hookType,
        const std::weak_ptr<C_ILuaGuardedState>& guardedState, const std::shared_ptr<hook::C_HookDispatcher>& hookDispatcher
    )
    {
        hookNamespace.set_function(name, [guardedState, hookDispatcher, hookType](const sol::protected_function& callback) {
            auto callbackID = luaL_ref(callback.lua_state(), LUA_REGISTRYINDEX);
            hookDispatcher->subscribe<arg_t...>(
                hookType, [guardedState, callbackID](arg_t ... args) {
                    const auto iface = guardedState.lock();
                    if (!iface) {
                        return;
                    }

                    const auto state_ptr = iface->getLuaState();
                    if (!state_ptr) {
                        return;
                    }

                    // Восстанавливаем функцию из registry
                    lua_State* L = state_ptr->lua_state();
                    lua_rawgeti(L, LUA_REGISTRYINDEX, callbackID);

                    if (!lua_isfunction(L, -1)) {
                        lua_pop(L, 1);
                        return;
                    }

                    // Проталкиваем аргументы на стек
                    push_args_to_stack(L, args...);

                    // Вызываем функцию
                    if (const auto argsCount = sizeof...(arg_t); lua_pcall(L, argsCount, 0, 0) != LUA_OK) {
                        dbg("Error in hook callback: %s", lua_tostring(L, -1));
                        lua_pop(L, 1);
                    }
                }
            );
        });
    }
     */

    // C++ porn...

    template<typename ... arg_t>
    void RegisterHookCallback(sol::table& hookNamespace, const std::string_view& name, hook::hook_id_t hookType,
        const std::weak_ptr<C_ILuaGuardedState>& guardedState, const std::shared_ptr<hook::C_HookDispatcher>& hookDispatcher
    )
    {
        hookNamespace.set_function(name, [guardedState, hookDispatcher, hookType](const sol::protected_function& callback) {
            hookDispatcher->subscribe<arg_t...>(
                hookType, [guardedState, callback](arg_t ... args) {
                    const auto iface = guardedState.lock();
                    if (!iface) {
                        dbg("Lua state is no longer available in hook callback");
                        return;
                    }

                    const auto state = iface->getLuaState(); // Thread-safe access to Lua state
                    if (const auto result = callback(args...); !result.valid()) {
                        dbg("Error in hook callback: %s", sol::error(result).what());
                    }
                }
            );
        });
    }

    bool C_LuaBindingHook::apply(const std::weak_ptr<C_ILuaGuardedState>& guardedState)
    {
        const auto tmp = guardedState.lock()->getLuaState();
        auto& luaState = *tmp;

        auto hookNamespace = luaState[HOOK_NAMESPACE_NAME].get_or_create<sol::table>();
        if (!hookNamespace.valid()) {
            dbg("Unable to create hook namespace!");
            return false;
        }

        const auto hookDispatcher = C_ServiceLocator::getInstance<hook::C_HookDispatcher>();

        RegisterHookCallback(
            hookNamespace,
            "set_on_render_start",
            static_cast<hook::hook_id_t>(hook::impl::hook_impl_type_e::ON_RENDER_START),
            guardedState,
            hookDispatcher
        );

        RegisterHookCallback(
            hookNamespace,
            "set_on_pre_update",
            static_cast<hook::hook_id_t>(hook::impl::hook_impl_type_e::ON_PRE_UPDATE),
            guardedState,
            hookDispatcher
        );

        RegisterHookCallback(
            hookNamespace,
            "set_on_update",
            static_cast<hook::hook_id_t>(hook::impl::hook_impl_type_e::ON_UPDATE),
            guardedState,
            hookDispatcher
        );

        RegisterHookCallback<const std::string_view&>(
            hookNamespace,
            "set_on_lua_dispose",
            static_cast<hook::hook_id_t>(hook::impl::hook_impl_type_e::ON_LUA_DISPOSE),
            guardedState,
            hookDispatcher
        );

        // Скрипт нужен исключительно для pull-style архи. Так lua-скрипты должны меньше влиять на производительность.
        if (const auto callbackAPI = luaState.require_script(script::CALLBACK_API_CHUNK_NAME,
            script::CALLBACK_API_SCRIPT); !callbackAPI.valid()) {
            dbg("Unable to load callback api script!");
            return false;
        }

        return true;
    }
} // lua