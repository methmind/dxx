//
// Created by sexey on 14.02.2026.
//
module;
#include <memory>

#include "debug/debug_output.h"

export module bootstrap;

import service.container;
import bootstrap.sdk;
import bootstrap.hook;

namespace bootstrap
{
    export std::unique_ptr<C_ServiceContainer> Build()
    {
        auto container = std::make_unique<C_ServiceContainer>();
        if (!InitializeSDK(container)) {
            dbg("InitializeSDK got:err = Unable to initiazlize SDK!");
            return nullptr;
        }

        if (!InitializeHooks(container)) {
            dbg("InitializeHooks got:err = Unable to initialize hooks!");
            return nullptr;
        }

        return container;
    }
}