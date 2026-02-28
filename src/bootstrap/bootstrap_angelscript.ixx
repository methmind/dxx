//
// Created by sexey on 17.02.2026.
//
module;
#include <memory>

#include "debug/debug_output.h"

export module bootstrap.angelscript;

import service.container;
import service.locator;
import as.manager;

import as.binding.imgui;
import as.binding.renderer;
import as.binding.hook;

import hook.dispatcher;
import hook.type;

import renderer;
import worker_queue;

import sdk.matrices_system;

namespace bootstrap
{
    using as_pre_renderer_queue_t = C_WorkerQueue;

    using queue_hook_subscription_t = hook::hook_subscription_t;

    export bool InitializeAngelscript(const std::unique_ptr<C_ServiceContainer>& services)
    {
        const auto asManager = services->add<as::C_ASManager>();
        if (!asManager->initialize()) {
            dbg("Unable to initialize AngelScript manager!");
            return false;
        }

        const auto asEngine = asManager->getEngine();
        if (!asEngine->addBinding(std::make_unique<as::C_ASBindingImgui>())) {
            dbg("Unable to initialize ImGUI binding!");
            return false;
        }

        const auto preRendererQueue = services->add<as_pre_renderer_queue_t>();
        if (!asEngine->addBinding(std::make_unique<as::C_ASBindingRenderer>(
            services->get<render::C_Renderer>(), preRendererQueue, services->get<sdk::C_MatricesSystem>()))) {
            dbg("Unable to initialize renderer binding!");
            return false;
        }

        if (!asEngine->addBinding(std::make_unique<as::C_ASBindingHook>())) {
            dbg("Unable to initialize hook binding!");
            return false;
        }

        services->add<queue_hook_subscription_t>(C_ServiceLocator::Get<hook::C_HookDispatcher>()->subscribe(
            static_cast<uint16_t>(hook::hook_type_e::ON_PRE_IMGUI_RENDER),
            [preRendererQueue]{ preRendererQueue->processCommands(); }
        ));

        return true;
    }
}