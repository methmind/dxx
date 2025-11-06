//
// Created by sexey on 30.10.2025.
//

#ifndef LUA_UTILS_H
#define LUA_UTILS_H

#include <sol/sol.hpp>

namespace lua
{
    std::optional<std::string> GetScriptPath(lua_State* L);
} // lua

#endif //LUA_UTILS_H