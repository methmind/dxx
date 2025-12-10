//
// Created by sexey on 26.10.2025.
//

#ifndef LUA_SCRIPT_MANAGER_H
#define LUA_SCRIPT_MANAGER_H

#include "lua_script_engine.h"
#include "lua_script_instance.h"
#include "lua_container_interface.h"
#include "hash/xxhash_wrapper.h"

namespace lua
{
    class C_LuaScriptManager final : public C_ILuaContainer, public std::enable_shared_from_this<C_LuaScriptManager>
    {
    public:
        using on_lua_dispose_callback_t = std::function<void(const std::string_view&)>;

    private:
        std::shared_ptr<C_LuaScriptEngine> engine_;
        //@note Сейчас модификация происходит только из GUI потока.
        std::unordered_map<std::string, std::shared_ptr<C_LuaScriptInstance>, xx_hashier_s, std::equal_to<>> scripts_;
        mutable on_lua_dispose_callback_t onLuaDisposeCallback_;

    public:

        std::shared_ptr<C_LuaScriptEngine> getEngine() { return this->engine_; }

        void disposeScript(const std::string_view& scriptPath);

        bool loadScript(const std::string_view& scriptPath);

        bool isScriptLoaded(const std::string_view& scriptPath) const;

        std::shared_ptr<C_LuaScriptInstance> getScriptInstance(const std::string_view& scriptPath) override;

        bool initialize(const on_lua_dispose_callback_t& onLuaDisposeCallback) const;

        C_LuaScriptManager() : engine_(std::make_shared<C_LuaScriptEngine>()) {}

        ~C_LuaScriptManager() override = default;
    };
} // lua

#endif //LUA_SCRIPT_MANAGER_H