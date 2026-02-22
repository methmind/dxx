//
// Created by sexey on 17.02.2026.
//
module;
#include <memory>

#include "debug/debug_output.h"

export module bootstrap.angelscript;

import service.container;
import as.manager;

import as.binding.imgui;
import as.binding.hook;

namespace bootstrap
{
    export bool InitializeAngelscript(const std::unique_ptr<C_ServiceContainer>& container)
    {
        const auto asManager = container->add<as::C_ASManager>();
        if (!asManager->initialize()) {
            dbg("Unable to initialize AngelScript manager!");
            return false;
        }

        const auto asEngine = asManager->getEngine();
        if (!asEngine->addBinding(std::make_unique<as::C_ASBindingImgui>())) {
            dbg("Unable to initialize ImGUI binding!");
            return false;
        }

        if (!asEngine->addBinding(std::make_unique<as::C_ASBindingHook>())) {
            dbg("Unable to initialize hook binding!");
            return false;
        }

        return true;
    }
}