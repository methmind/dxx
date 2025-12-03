//
// Created by sexey on 03.12.2025.
//

#include "lua_binding_source_engine.h"

#include "debug/debug_output.h"
#include "sdk/singleton/sdk_source2_engine_to_client.h"
#include "service_locator/service_locator.h"

namespace lua::binding
{
    bool C_LuaBindingSourceEngine::apply(const std::weak_ptr<C_ILuaGuardedState>& guardedState)
    {
        const auto tmp = guardedState.lock()->getLuaState();
        auto& luaState = *tmp;

        auto engineNamespace = luaState[ENGINE_NAMESPACE_NAME].get_or_create<sol::table>();
        if (!engineNamespace.valid()) {
            dbg("Unable to create engine namespace!");
            return false;
        }

        engineNamespace.set_function("is_in_game", [] {
           return C_ServiceLocator::getInstance<sdk::singleton::C_Source2EngineToClient>()->isInGame();
        });

        return true;
    }
} // lua