//
// Created by sexey on 17.02.2026.
//
module;
#include <memory>

#include "debug/debug_output.h"

export module bootstrap.angelscript;

import service.container;
import as.manager;

namespace bootstrap
{
    export bool InitializeAngelscript(const std::unique_ptr<C_ServiceContainer>& container)
    {
        const auto asManager = container->add<as::C_ASManager>();
        if (!asManager->initialize()) {
            dbg("Unable to initialize AngelScript manager!");
            return false;
        }

        return true;
    }
}