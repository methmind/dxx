//
// Created by sexey on 26.10.2025.
//

#include "lua_script_manager.h"

#include <filesystem>
#include <format>

#include "debug/debug_output.h"
#include "hook/hook_dispatcher.h"
#include "hook/impl/hook_impl_type.h"

namespace lua
{
    void C_LuaScriptManager::disposeScript(const std::string_view& scriptPath)
    {
        if (!isScriptLoaded(scriptPath)) {
            return;
        }

        // Deadlock prevention: invoke hooks before acquiring lua state lock
        this->onLuaDisposeCallback_(scriptPath);

        std::shared_ptr<C_LuaScriptInstance> scriptToDispose;

        {
            auto scripts = this->scripts_.lock();
            if (auto it = scripts->find(scriptPath.data()); it != scripts->end()) {
                scriptToDispose = it->second;
                scripts->erase(it);
            }
        } // Release scripts lock

        if (scriptToDispose) {
            auto locker = this->engine_->getLuaState(); // Ensure thread safety during disposal (destructor calls Lua)
            scriptToDispose.reset();
        }
    }

    bool C_LuaScriptManager::loadScript(const std::string_view& scriptPath)
    {
        try {
            if (isScriptLoaded(scriptPath)) {
                return true;
            }

            if (!std::filesystem::exists(scriptPath)) {
                dbg("Script file does not exist!");
                return false;
            }

            auto newInstance = std::make_shared<C_LuaScriptInstance>(this->engine_);

            // Scope for Lua initialization
            {
                const auto luaState = this->engine_->getLuaState();

                auto payloadData = luaState->load_file(scriptPath.data());
                if (!payloadData.valid()) {
                    dbg("Unable to load user script: %s!", sol::error(payloadData).what());
                    return false;
                }

                const sol::protected_function_result payloadResult = payloadData();
                if (!payloadResult.valid()) {
                    throw std::runtime_error(std::format("[{}] got:err = {}", scriptPath.data(), sol::error(payloadResult).what()));
                }

                if (payloadResult.get_type() != sol::type::table) {
                    throw std::runtime_error("Invalid lua structure!");
                }

                if (!newInstance->initialize(payloadResult)) {
                    throw std::runtime_error("Script execution failed!");
                }
            } // Release Lua lock

            // Scope for Script registration
            {
                auto scripts = this->scripts_.lock();
                if (scripts->contains(scriptPath.data())) {
                     // Maybe it was added while we were initializing?
                    throw std::runtime_error("Script already exists!");
                }
                scripts->emplace(scriptPath.data(), newInstance);
            }

            dbg("Lua script loaded: %s", scriptPath.data());
            return true;
        } catch (const std::exception& ex) {
            dbg("Critical exception: %s", ex.what());
            return false;
        }
    }

    bool C_LuaScriptManager::isScriptLoaded(const std::string_view& scriptPath) const
    {
        auto scripts = this->scripts_.lock_shared();
        return scripts->contains(scriptPath.data());
    }

    std::shared_ptr<C_LuaScriptInstance> C_LuaScriptManager::getScriptInstance(const std::string_view& scriptPath)
    {
        auto scripts = this->scripts_.lock_shared();
        const auto it = scripts->find(scriptPath.data());
        if (it == scripts->end()) {
            return nullptr;
        }

        return it->second;
    }

    bool C_LuaScriptManager::initialize(const on_lua_dispose_callback_t& onLuaDisposeCallback) const
    {
        if (!this->engine_->initialize()) {
            dbg("Unable to initialize C_LuaScriptEngine!");
            return false;
        }

        this->onLuaDisposeCallback_ = onLuaDisposeCallback;
        return true;
    }
} // lua