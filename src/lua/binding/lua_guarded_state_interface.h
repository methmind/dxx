//
// Created by sexey on 13.11.2025.
//

#ifndef DXX_DLC_LUA_GUARDED_INTERFACE_H
#define DXX_DLC_LUA_GUARDED_INTERFACE_H

#include "cs_plain_guarded.h"
#include "sol/sol.hpp"

namespace lua::binding
{
    using lua_guarded_state_t = libguarded::plain_guarded<sol::state>;

    using lua_guarded_handle_t = typename lua_guarded_state_t::handle;

    class C_ILuaGuardedState
    {
    public:
        virtual lua_guarded_handle_t getLuaState() = 0;

        virtual ~C_ILuaGuardedState() = default;
    };
}

#endif //DXX_DLC_LUA_GUARDED_INTERFACE_H