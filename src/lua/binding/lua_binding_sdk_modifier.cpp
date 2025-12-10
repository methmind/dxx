//
// Created by sexey on 06.12.2025.
//

#include "lua_binding_sdk_modifier.h"

#include "sdk/datatype/sdk_dota_buff.h"
#include "sdk/datatype/sdk_dota_modifier_manager.h"
#include "lua_binding_utl_vector.h"

namespace lua::binding
{
    bool C_LuaBindingSdkModifier::apply(const std::weak_ptr<C_ILuaGuardedState>& guardedState)
    {
        const auto tmp = guardedState.lock()->getLuaState();
        auto& luaState = *tmp;

        auto luaDotaBuff = luaState.new_usertype<sdk::datatype::C_DotaBuff>(
            "C_DotaBuff", sol::no_constructor
        );
        luaDotaBuff.set_function("get_name", &sdk::datatype::C_DotaBuff::getName);

        auto luaModifierManager = luaState.new_usertype<sdk::datatype::C_DotaModifierManager>(
            "C_DotaModifierManager", sol::no_constructor
        );
        luaModifierManager.set_function("get_assigned_hero", &sdk::datatype::C_DotaModifierManager::getAssignedEntity);
        luaModifierManager.set_function("get_modifiers", &sdk::datatype::C_DotaModifierManager::getModifiers);

        return true;
    }
} // lua