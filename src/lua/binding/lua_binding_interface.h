//
// Created by sexey on 26.11.2025.
//

#ifndef DXX_DLC_LUA_BINDING_INTERFACE_H
#define DXX_DLC_LUA_BINDING_INTERFACE_H

#include "lua_guarded_state_interface.h"

namespace lua::binding
{
    class C_ILuaBinding
    {
    public:

        virtual bool apply(const std::weak_ptr<C_ILuaGuardedState>& guardedState) = 0;

        virtual ~C_ILuaBinding() = default;
    };
}

#endif //DXX_DLC_LUA_BINDING_INTERFACE_H