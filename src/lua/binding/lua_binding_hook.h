//
// Created by sexey on 13.11.2025.
//

#ifndef DXX_DLC_LUA_BINDING_HOOK_H
#define DXX_DLC_LUA_BINDING_HOOK_H

#include "lua/lua_syncer_interface.h"
#include "sol/sol.hpp"

/*
 *@brief Do not use any stuff from this anywhere except callback_api.lua
*/

namespace lua::binding
{
    constexpr auto HOOK_NAMESPACE_NAME = "hook";

    bool RegisterHookApi(sol::state& luaState, std::weak_ptr<C_ILuaSyncerInterface> syncer);
} // lua

#endif //DXX_DLC_LUA_BINDING_HOOK_H