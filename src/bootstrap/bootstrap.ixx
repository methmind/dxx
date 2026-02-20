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
import bootstrap.gui;
import bootstrap.angelscript;

import renderer;
import gui.regedit;

import bind_system;

namespace bootstrap
{
    export std::unique_ptr<C_ServiceContainer> Build()
    {
        auto container = std::make_unique<C_ServiceContainer>();
        if (!InitializeSDK(container)) {
            dbg("InitializeSDK got:err = Unable to initialize SDK!");
            return nullptr;
        }

        if (!InitializeHooks(container)) {
            dbg("InitializeHooks got:err = Unable to initialize hooks!");
            return nullptr;
        }

        container->add<input::C_BindSystem>();
        container->add<render::C_Renderer>();

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