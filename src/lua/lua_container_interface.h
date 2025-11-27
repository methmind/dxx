//
// Created by sexey on 30.10.2025.
//

#ifndef LUA_CONTAINER_INTERFACE_H
#define LUA_CONTAINER_INTERFACE_H

#include "lua_script_instance.h"

namespace lua
{
    class C_ILuaContainer
    {
    public:

        virtual std::shared_ptr<C_LuaScriptInstance> getScriptInstance(const std::string_view& scriptPath) = 0;

        virtual ~C_ILuaContainer() = default;
    };
}

#endif //LUA_CONTAINER_INTERFACE_H