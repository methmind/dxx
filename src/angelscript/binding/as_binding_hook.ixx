//
// Created by sexey on 22.02.2026.
//
module;
#include <format>
#include <memory>

#include "imgui.h"
#include "asbind20/asbind.hpp"
#include "debug/debug_output.h"

export module as.binding.hook;

import service.locator;

import as.binding;
import as.engine_interface;
import as.binding.memory;

import hook.dispatcher;
import hook.type;

namespace as
{
    constexpr auto AS_HOOK_NAMESPACE_NAME = "hook";

    using hook_subscription_t = C_SharedPtr<hook::hook_subscription_t>;

    export class C_ASBindingHook : public C_IASBinding
    {
    public:
        C_ASBindingHook() = default;

        [[nodiscard]] bool apply(const std::weak_ptr<C_IASEngine>& engineWeak) override
        {
            const auto enginePtr = engineWeak.lock();
            if (!enginePtr) {
                return false;
            }

            const auto engine = enginePtr->getEngine();
            engine->SetDefaultNamespace(AS_HOOK_NAMESPACE_NAME);

            asbind20::ref_class<hook_subscription_t>(engine, "hook_subscription_t")
                .addref(&hook_subscription_t::addRef)
                .release(&hook_subscription_t::release);

            registerHookCallback<static_cast<hook::hook_id_t>(hook::hook_type_e::ON_IMGUI_RENDER), ImDrawList*>(
                engine,
                "setOnRender",
                "void onRender(render::C_Frame@)"
            );

            registerHookCallback<static_cast<hook::hook_id_t>(hook::hook_type_e::ON_LEVEL_INIT)>(
                engine,
                "setOnLevelInit",
                "void onLevelInit()"
            );

            registerHookCallback<static_cast<hook::hook_id_t>(hook::hook_type_e::ON_LEVEL_SHUTDOWN)>(
                engine,
                "setOnLevelShutdown",
                "void onLevelShutdown()"
            );

            engine->SetDefaultNamespace("");
            return true;
        }

    private:
        static std::string ExtractFuncDefName(const std::string& decl)
        {
            const auto parenPos = decl.find('(');
            if (parenPos == std::string::npos) {
                return "";
            }

            const auto spacePos = decl.rfind(' ', parenPos);
            if (spacePos == std::string::npos) {
                return "";
            }

            return decl.substr(spacePos + 1, parenPos - spacePos - 1);
        }

        template<hook::hook_id_t hook_t, typename ... arg_t>
        void registerHookCallback(
            asIScriptEngine* engine,
            const std::string& funcName,
            const std::string& funcDefDecl
        )
        {
            const auto funcDefType = ExtractFuncDefName(funcDefDecl);
            if (funcDefType.empty()) {
                assert(std::format("Invalid funcdef declaration format: {}", funcDefDecl).c_str());
                return;
            }

            if (engine->GetTypeInfoByDecl(funcDefDecl.c_str()) == nullptr) {
                engine->RegisterFuncdef(funcDefDecl.c_str());
            }

            auto subscribeProxy = [](asIScriptFunction* callback) -> hook_subscription_t* {
                if (!callback) {
                    asbind20::set_script_exception("Callback function cannot be null!");
                    return nullptr;
                }

                auto safeCallback = std::shared_ptr<asIScriptFunction>(callback, [](const asIScriptFunction* function) {
                    if (function) {
                        // ReSharper disable once CppExpressionWithoutSideEffects
                        function->Release();
                    }
                });

                const auto scriptEngine = callback->GetEngine();
                auto sub = C_ServiceLocator::Get<hook::C_HookDispatcher>()->subscribe<arg_t...>(hook_t, [scriptEngine, safeCallback](arg_t ... args) {
                    const auto ctx = static_cast<C_IASEngine*>(scriptEngine->GetUserData(ENGINE_USERDATA_ID))->getContext();
                    const auto result = asbind20::script_invoke<void>(ctx.get(), safeCallback.get(), std::forward<arg_t>(args)...);
                    if (!result) {
                        dbg("[AngelScript] Hook Exception: {}", ctx->GetExceptionString());
                    }
                });

                return new hook_subscription_t(std::make_shared<hook::hook_subscription_t>(std::move(sub)));
            };

            asbind20::global(engine).function(
                std::format("hook_subscription_t@ {}({}@)", funcName, funcDefType),
                subscribeProxy
            );
        }
    };
}
