//
// Created by sexey on 19.02.2026.
//
module;
#include <memory>

#include "angelscript.h"
#include "debug/debug_output.h"

export module as.instance;

import as.engine;

namespace as
{
    export class C_ASInstance
    {
    public:
        ~C_ASInstance()
        {
            if (const auto engine = this->engine_.lock(); this->exitPoint_ && engine) {
                const auto ctx = engine->getContext();
                ctx->Prepare(this->exitPoint_);
                ctx->Execute();
            }

            if (this->module_) {
                this->module_->Discard();
            }
        }

        explicit C_ASInstance(const std::shared_ptr<C_AScriptEngine>& engine) :
            module_(nullptr), entryPoint_(nullptr), exitPoint_(nullptr), engine_(engine) {}

        [[nodiscard]] bool initialize(const std::string& moduleName)
        {
            const auto engine = this->engine_.lock();
            this->module_ = engine->getEngine()->GetModule(moduleName.c_str());
            if (!this->module_) {
                dbg("Unable to get module: {}!", moduleName.c_str());
                return false;
            }

            this->entryPoint_ = this->module_->GetFunctionByDecl("void main()");
            this->exitPoint_  = this->module_->GetFunctionByDecl("void onDispose()");
            if (!this->entryPoint_) {
                return true;
            }

            const auto ctx = engine->getContext();
            ctx->Prepare(this->entryPoint_);
            switch (const auto err = ctx->Execute()) {
                case asEXECUTION_FINISHED:
                    return true;
                case asEXECUTION_EXCEPTION:
                    dbg("Script threw an exception: {}!", ctx->GetExceptionString());
                    return false;
                default:
                    dbg("Script execution failed with error code: {}!", err);
                    return false;
            }
        }

    private:
        asIScriptModule* module_;
        asIScriptFunction* entryPoint_;
        asIScriptFunction* exitPoint_;

        std::weak_ptr<C_AScriptEngine> engine_;
    };
}