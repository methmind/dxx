//
// Created by sexey on 19.02.2026.
//
module;
#include <filesystem>
#include <memory>
#include <unordered_map>

#include "debug/debug_output.h"
#include "scriptbuilder/scriptbuilder.h"

export module as.manager;

import as.engine;
import as.instance;
import xxhash.wrapper;

namespace as
{
    export class C_ASManager
    {
    public:
        ~C_ASManager() = default;

        C_ASManager() : engine_(std::make_shared<C_AScriptEngine>()) {}

        bool initialize() const
        {
            if (!this->engine_->initialize()) {
                dbg("Unable to initialize AngelScript engine!");
                return false;
            }

            return true;
        }

        void disposeScript(const std::string_view& scriptPath)
        {
            if (!isScriptLoaded(scriptPath)) {
                return;
            }

            this->scripts_.erase(scriptPath.data());
            this->engine_->getEngine()->GarbageCollect();
        }

        bool loadScript(const std::string_view& scriptPath)
        {
            if (isScriptLoaded(scriptPath)) {
                return true;
            }

            if (!std::filesystem::exists(scriptPath)) {
                dbg("Script file does not exist!");
                return false;
            }

            CScriptBuilder builder;
            if (const auto err = builder.StartNewModule(this->engine_->getEngine(), scriptPath.data()); err < asSUCCESS) {
                dbg("Failed to start new module for script: {} with error code: {}", scriptPath.data(), err);
                return false;
            }

            if (const auto err = builder.AddSectionFromFile(scriptPath.data()); err < asSUCCESS) {
                dbg("Failed to add script section from file: {} with error code: {}", scriptPath.data(), err);
                return false;
            }

            if (const auto err = builder.BuildModule(); err < asSUCCESS) {
                dbg("Failed to build module for script: {} with error code: {}", scriptPath.data(), err);
                return false;
            }

            if (auto [it, _] = this->scripts_.emplace(scriptPath, std::make_shared<C_ASInstance>(this->engine_));
                !it->second->initialize(scriptPath.data())) {
                dbg("Failed to initialize script instance for script: {}", scriptPath.data());
                this->scripts_.erase(scriptPath.data());
                return false;
            }

            dbg("AngelScript loaded: {}", scriptPath.data());
            return true;
        }

        [[nodiscard]] bool isScriptLoaded(const std::string_view& scriptPath) const { return this->scripts_.contains(scriptPath); }

        auto getEngine() { return this->engine_; }

    private:
        std::shared_ptr<C_AScriptEngine> engine_;
        std::unordered_map<std::string, std::shared_ptr<C_ASInstance>, xx_hasher_s, std::equal_to<>> scripts_;
    };
}
