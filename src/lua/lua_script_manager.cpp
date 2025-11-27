//
// Created by sexey on 26.10.2025.
//

#include "lua_script_manager.h"

#include <filesystem>
#include <format>

#include "debug/debug_output.h"
#include "hook/hook_dispatcher.h"
#include "hook/impl/hook_impl_type.h"
#include "service_locator/service_locator.h"

namespace lua
{
    void C_LuaScriptManager::disposeScript(const std::string_view& scriptPath)
    {
        // Deadlock prevention: invoke hooks before acquiring lua state lock (cuz in listener we may have another lock)
        C_ServiceLocator::getInstance<hook::C_HookDispatcher>()->invoke<const std::string_view&>(
            static_cast<hook::hook_id_t>(hook::impl::hook_impl_type_e::ON_LUA_DISPOSE), scriptPath
        );

        auto locker = this->engine_->getLuaState(); // Ensure thread safety during disposal
        this->scripts_.erase(scriptPath.data());
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

            const auto luaState = this->engine_->getLuaState();
            auto payloadData = luaState->load_file(scriptPath.data());

            if (!payloadData.valid()) {
                dbg("Unable to load user script: %s!", sol::error(payloadData).what());
                return false;
            }

            auto [it, inserted] = this->scripts_.emplace(scriptPath, std::make_shared<C_LuaScriptInstance>());
            if (!inserted) {
                throw std::runtime_error("Script already exists!");
            }

            const sol::protected_function_result payloadResult = payloadData();
            if (!payloadResult.valid()) {
                throw std::runtime_error(std::format("[{}] got:err = {}", scriptPath.data(), sol::error(payloadResult).what()));
            }

            if (payloadResult.get_type() != sol::type::table) {
                throw std::runtime_error("Invalid lua structure!");
            }

            if (!it->second->initialize(payloadResult)) {
                throw std::runtime_error("Script execution failed!");
            }

            dbg("Lua script loaded: %s", scriptPath.data());
            return true;
        } catch (const std::exception& ex) {
            dbg("Critical exception: %s", ex.what());
            this->scripts_.erase(scriptPath.data());
            return false;
        }
    }

    bool C_LuaScriptManager::isScriptLoaded(const std::string_view& scriptPath) const
    {
        return this->scripts_.contains(scriptPath.data());
    }

    std::shared_ptr<C_LuaScriptInstance> C_LuaScriptManager::getScriptInstance(const std::string_view& scriptPath)
    {
        const auto it = this->scripts_.find(scriptPath.data());
        if (it == this->scripts_.end()) {
            return nullptr;
        }

        return it->second;
    }

    bool C_LuaScriptManager::initialize() const
    {
        if (!this->engine_->initialize()) {
            dbg("Unable to initialize C_LuaScriptEngine!");
            return false;
        }

        return true;
    }
} // lua