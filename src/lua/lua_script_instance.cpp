//
// Created by sexey on 26.10.2025.
//

#include "lua_script_instance.h"

#include "debug/debug_output.h"

namespace lua
{
    void C_LuaScriptInstance::addDependency(const dependency_t& dependency)
    {
        this->dependencies_.lock()->push_back(dependency);
    }

    bool C_LuaScriptInstance::initialize(const sol::protected_function_result& instance)
    {
        auto module = instance.get<sol::table>();
        if (!module.valid()) {
            return false;
        }

        if (this->entryPoint_= module[LUA_ENTRY_FUNCTION_NAME];
            !this->entryPoint_.valid() || this->entryPoint_.get_type() != sol::type::function) {
            return false;
        }

        if (const auto result = this->entryPoint_(0); !result.valid()) {
            dbg("[Lua] error: %s", sol::error(result).what());
            return false;
        }

        return true;
    }

    C_LuaScriptInstance::~C_LuaScriptInstance()
    {
        if (this->entryPoint_.valid()) {
            this->entryPoint_(1337);
        }
    }
} // lua