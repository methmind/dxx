//
// Created by sexey on 17.02.2026.
//
module;
#include <memory>
#include <atomic>

#include "angelscript.h"
#include "datetime/datetime.h"
#include "scriptstdstring/scriptstdstring.h"
#include "debug/debug_output.h"
#include "scriptany/scriptany.h"
#include "scriptarray/scriptarray.h"
#include "scriptdictionary/scriptdictionary.h"
#include "scriptfile/scriptfile.h"
#include "scripthandle/scripthandle.h"
#include "scripthelper/scripthelper.h"
#include "scriptmath/scriptmath.h"
#include "scriptsocket/scriptsocket.h"

export module as.engine;

import as.binding;
import as.context_pool;

namespace as
{
    export class C_AScriptEngine : public std::enable_shared_from_this<C_AScriptEngine>
    {
    public:
        ~C_AScriptEngine()
        {
            if (this->isMultithreadInitialized_) {
                asUnprepareMultithread();
            }

            if (this->engine_) {
                this->engine_->ShutDownAndRelease();
            }
        }

        C_AScriptEngine() : engine_(nullptr) {}

        bool initialize()
        {
            if (const auto err = asPrepareMultithread(); err < asSUCCESS) {
                dbg("asPrepareMultithread got:err = {}!", err);
                return false;
            }

            this->isMultithreadInitialized_ = true;
            if (this->engine_ = asCreateScriptEngine(ANGELSCRIPT_VERSION); !this->engine_) {
                dbg("asCreateScriptEngine got:err = Unable to create script engine!");
                return false;
            }

            this->engine_->SetMessageCallback(asFUNCTION(MessageCallback), nullptr, asCALL_CDECL);
            this->contextPool_ = std::make_unique<C_ASContextPool>(this->engine_);

            initializeDefaultAddons();
            this->engine_->RegisterGlobalFunction("void print(const string& in)", asFUNCTION(PrintOverride), asCALL_CDECL);

            return true;
        }

        void addBinding(std::unique_ptr<C_IASBinding> binding)
        {
            binding->apply(this->engine_);
            this->bindings_.push_back(std::move(binding));
        }

        [[nodiscard]] asIScriptEngine* getEngine() const { return this->engine_; }

        [[nodiscard]] as_context_t getContext() const { return this->contextPool_->getContext(); }

    private:
        static void PrintOverride(const std::string& msg)
        {
            dbg("[AngelScript]: {}", msg);
        }

        static void MessageCallback(const asSMessageInfo* msg, void* param)
        {
            const char* type;
            switch (msg->type) {
                case asMSGTYPE_WARNING: type = "WARN"; break;
                case asMSGTYPE_INFORMATION: type = "INFO"; break;
                default: type = "ERR"; break;
            }

            dbg("{} ({} {}) {} : {}", msg->section, msg->row, msg->col, type, msg->message);
        }

        void initializeDefaultAddons() const
        {
            RegisterScriptArray(this->engine_, true);
            RegisterStdString(this->engine_);
            RegisterStdStringUtils(this->engine_);
            RegisterScriptDictionary(this->engine_);
            RegisterScriptMath(this->engine_);
            RegisterScriptFile(this->engine_);
            RegisterScriptAny(this->engine_);
            RegisterScriptHandle(this->engine_);
            RegisterScriptSocket(this->engine_);
            RegisterExceptionRoutines(this->engine_);
            RegisterScriptDateTime(this->engine_);
        }

        bool isMultithreadInitialized_ = false;
        asIScriptEngine* engine_;
        std::list<std::unique_ptr<C_IASBinding>> bindings_;

        std::unique_ptr<C_ASContextPool> contextPool_;
    };
}