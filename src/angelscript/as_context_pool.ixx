//
// Created by sexey on 19.02.2026.
//
module;
#include <memory>
#include <vector>

#include "angelscript.h"
#include "cs_plain_guarded.h"

export module as.context_pool;

namespace as
{
    struct context_deleter_s
    {
        asIScriptEngine* engine;
        void operator()(asIScriptContext* ctx) const
        {
            if (ctx && this->engine) {
                this->engine->ReturnContext(ctx);
            }
        }
    };

    export using as_context_t = std::unique_ptr<asIScriptContext, context_deleter_s>;

    export class C_ASContextPool
    {
    public:
        ~C_ASContextPool()
        {
            for (const auto pool = this->pool_.lock(); const auto* ctx : *pool) {
                // ReSharper disable once CppExpressionWithoutSideEffects
                ctx->Release();
            }
        }

        explicit C_ASContextPool(asIScriptEngine* engine) : engine_(engine)
        {
            this->engine_->SetContextCallbacks(
                &C_ASContextPool::onContextRequest,
                &C_ASContextPool::onContextReturn,
                this
            );
        }

        [[nodiscard]] as_context_t getContext() const
        {
            return as_context_t(this->engine_->RequestContext(), context_deleter_s{this->engine_});
        }

    private:
        static asIScriptContext* onContextRequest(asIScriptEngine* engine, void* userData)
        {
            auto self = static_cast<C_ASContextPool*>(userData);
            asIScriptContext* ctx = nullptr;

            auto pool = self->pool_.lock();
            if (!pool->empty()) {
                ctx = pool->back();
                pool->pop_back();
            }

            if (!ctx) {
                ctx = engine->CreateContext();
            }

            return ctx;
        }

        static void onContextReturn(asIScriptEngine* engine, asIScriptContext* ctx, void* userData)
        {
            static_cast<C_ASContextPool*>(userData)->pool_.lock()->push_back(ctx);
        }

        asIScriptEngine* engine_;
        libguarded::plain_guarded<std::vector<asIScriptContext*>> pool_;
    };
}
