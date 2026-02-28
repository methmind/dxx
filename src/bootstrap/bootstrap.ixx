//
// Created by sexey on 14.02.2026.
//
module;
#include <memory>
#include <windows.h>

#include "debug/debug_output.h"

export module bootstrap;

import service.container;
import bootstrap.sdk;
import bootstrap.hook;
import bootstrap.gui;
import bootstrap.angelscript;

import renderer;
import gui.regedit;

import bind_system;
import worker_queue;

namespace bootstrap
{
    export std::unique_ptr<C_ServiceContainer> Build()
    {
        auto container = std::make_unique<C_ServiceContainer>();
        if (!InitializeHooks(container)) {
            dbg("InitializeHooks got:err = Unable to initialize hooks!");
            return nullptr;
        }

        if (!InitializeSDK(container)) {
            dbg("InitializeSDK got:err = Unable to initialize SDK!");
            return nullptr;
        }

        if (!SetupHooks(container)) {
            dbg("SetupHooks got:err = Unable to create hooks!");
            return nullptr;
        }

        container->add<input::C_BindSystem>();
        const auto renderer = container->add<render::C_Renderer>();
        while (!renderer->isContextInitialized()) {
            Sleep(1);
        }

        if (!InitializeAngelscript(container)) {
            dbg("InitializeAngelscript got:err = Unable to initialize Angelscript!");
            return nullptr;
        }

        if (!InitializeGUI(container)) {
            dbg("InitializeGUI got:err = Unable to initialize GUI!");
            return nullptr;
        }

        return container;
    }
}