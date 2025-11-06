//
// Created by sexey on 26.10.2025.
//

#include "lua_script_manager.h"

#include <filesystem>
#include <format>

#include "debug/debug_output.h"

namespace lua
{
    bool C_LuaScriptManager::bindWidget(const std::string_view& luaID, const gui::widget_ptr_t& widget)
    {
        const auto it = this->scripts_.find(luaID);
        if (it == this->scripts_.end()) {
            dbg("Unable to find lua script: %s!", luaID.data());
            return false;
        }

        it->second->addWidget(widget);
        return true;
    }

    void C_LuaScriptManager::disposeScript(const std::string_view& scriptPath)
    {
        this->scripts_.erase(scriptPath);
    }

    bool C_LuaScriptManager::loadScript(const std::string_view& scriptPath)
    {
        try {
            if (this->scripts_.contains(scriptPath)) {
                return true;
            }

            if (!std::filesystem::exists(scriptPath)) {
                dbg("Script file does not exist!");
                return false;
            }

            auto payloadData = this->engine_.getLuaState().load_file(scriptPath.data());
            if (!payloadData.valid()) {
                dbg("Script load failed!");
                return false;
            }

            auto [it, inserted] = this->scripts_.emplace(scriptPath,
                std::make_unique<C_LuaScriptInstance>(this->widgetRegedit_)
            );
            if (!inserted) {
                throw std::runtime_error("Script already exists!");
            }

            sol::protected_function_result payloadResult = payloadData();
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
            this->scripts_.erase(scriptPath);
            return false;
        }
    }

    bool C_LuaScriptManager::initialize()
    {
        if (!this->engine_.initialize(this->widgetRegedit_, weak_from_this())) {
            dbg("Unable to initialize C_LuaScriptEngine!");
            return false;
        }

        return true;
    }
} // lua