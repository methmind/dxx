//
// Created by sexey on 13.11.2025.
//

#include "lua_binding_hook.h"

#include <format>

#include "debug/debug_output.h"
#include "hook/hook_dispatcher.h"
#include "hook/impl/hook_impl_on_render_start.h"
#include "service_locator/service_container.h"
#include "service_locator/service_locator.h"

namespace lua::binding
{
    bool RegisterHookApi(sol::state& luaState, std::weak_ptr<C_ILuaSyncerInterface> syncer)
    {
        auto hookNamespace = luaState[HOOK_NAMESPACE_NAME].get_or_create<sol::table>();
        if (!hookNamespace.valid()) {
            dbg("Unable to create hook namespace!");
            return false;
        }

        //todo This is unsafe, need to figure out a better way to do this
        hookNamespace.set_function("subscribe",
            [syncer](const std::string& hookSID, const sol::function& callback) {
                C_ServiceLocator::getInstance<hook::C_HookDispatcher>()->subscribe(
                    hookSID, true, [callback, syncer]<typename ... T>(T&& ... args) {
                        const auto syncerPtr = syncer.lock();
                        if (!syncerPtr) {
                            dbg("Unable to lock C_ILuaSyncerInterface!");
                            return;
                        }

                        auto locker = syncerPtr->luaStateLocker();
                        if (const sol::protected_function_result result = callback(std::forward<T>(args)...);
                            !result.valid()) {
                            dbg("Error in hook callback: %s", sol::error(result).what());
                        }
                    }
                );
            }
        );

        return true;
    }
} // lua