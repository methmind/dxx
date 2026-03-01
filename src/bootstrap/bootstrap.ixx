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
import bootstrap.menu;
import bootstrap.angelscript;

import renderer;
import gui.regedit;

import bind_system;
import worker_queue;

namespace bootstrap
{
    export std::unique_ptr<C_ServiceContainer> Build()
    {
        auto services = std::make_unique<C_ServiceContainer>();
        if (!InitializeHooks(services)) {
            dbg("InitializeHooks got:err = Unable to initialize hooks!");
            return nullptr;
        }

        if (!InitializeSDK(services)) {
            dbg("InitializeSDK got:err = Unable to initialize SDK!");
            return nullptr;
        }

        if (!SetupHooks(services)) {
            dbg("SetupHooks got:err = Unable to create hooks!");
            return nullptr;
        }

        services->add<input::C_BindSystem>();
        const auto renderer = services->add<render::C_Renderer>();
        while (!renderer->isContextInitialized()) {
            Sleep(1);
        }

        if (!InitializeGUI(services)) {
            dbg("InitializeGUI got:err = Unable to initialize GUI!");
            return nullptr;
        }

        if (!InitializeAngelscript(services)) {
            dbg("InitializeAngelscript got:err = Unable to initialize Angelscript!");
            return nullptr;
        }

        if (!InitializeMenu(services)) {
            dbg("InitializeGUI got:err = Unable to initialize GUI!");
            return nullptr;
        }

        return services;
    }
}