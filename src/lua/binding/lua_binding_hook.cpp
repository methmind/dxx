//
// Created by sexey on 13.11.2025.
//

#include "lua_binding_hook.h"

#include <format>

#include "debug/debug_output.h"
#include "hook/hook_dispatcher.h"
#include "hook/impl/hook_impl_on_render_start.h"
#include "hook/impl/hook_impl_type.h"
#include "service_locator/service_container.h"
#include "service_locator/service_locator.h"

namespace lua::binding
{
    // C++ porn...

    template<typename ... arg_t>
    void RegisterHookCallback(sol::table& hookNamespace, const std::string_view& name, hook::hook_id_t hookType,
        const std::weak_ptr<C_ILuaGuardedState>& syncer, const std::shared_ptr<hook::C_HookDispatcher>& hookDispatcher
    )
    {
        hookNamespace.set_function(name, [syncer, hookDispatcher, hookType](const sol::function& callback) {
            hookDispatcher->subscribe<arg_t...>(
                hookType, [syncer, callback](arg_t ... args) {
                    const auto iface = syncer.lock();
                    if (!iface) {
                        dbg("Lua state is no longer available in hook callback");
                        return;
                    }

                    const auto guardedState = iface->getLuaState(); // Thread-safe access to Lua state
                    if (const sol::protected_function_result result = callback(args...);
                        !result.valid()) {
                        dbg("Error in hook callback: %s", sol::error(result).what());
                    }
                }
            );
        });
    }

    bool RegisterHookApi(const std::weak_ptr<C_ILuaGuardedState>& syncer)
    {
        const auto guardedState = syncer.lock()->getLuaState();
        auto& luaState = *guardedState;

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
            syncer,
            hookDispatcher
        );

        RegisterHookCallback<const std::string_view&>(
            hookNamespace,
            "set_on_lua_dispose",
            static_cast<hook::hook_id_t>(hook::impl::hook_impl_type_e::ON_LUA_DISPOSE),
            syncer,
            hookDispatcher
        );

        return true;
    }
} // lua