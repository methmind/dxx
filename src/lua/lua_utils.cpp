//
// Created by sexey on 30.10.2025.
//

#include "lua_utils.h"

namespace lua
{
    std::string GetScriptPath(lua_State* L)
    {
        lua_Debug ar;
        std::string script_name;
        int level = 1, max_level = 0;

        while (lua_getstack(L, level, &ar)) {
            ++level;
        }
        max_level = level - 1;

        if (max_level > 0 && lua_getstack(L, max_level, &ar)) {
            lua_getinfo(L, "S", &ar);
            if (ar.source && ar.source[0] == '@') {
                script_name = ar.source + 1;
            }
        }

        return script_name;
    }
} // lua