//
// Created by sexey on 01.12.2025.
//

#include "lua_binding_sdk_schema.h"

#include "sdk/interface/sdk_schema_system.h"

namespace lua::binding
{
    bool C_LuaBindingSdkSchema::apply(const std::weak_ptr<C_ILuaGuardedState>& guardedState)
    {
        const auto tmp = guardedState.lock()->getLuaState();
        auto& luaState = *tmp;

        auto luaSchemaClassInfo = luaState->new_usertype<sdk::iface::C_SchemaClassInfo>("C_SchemaClassInfo", sol::no_constructor);
        luaSchemaClassInfo.set_function("get_name", &sdk::iface::C_SchemaClassInfo::getName);

        return true;
    }
} // lua