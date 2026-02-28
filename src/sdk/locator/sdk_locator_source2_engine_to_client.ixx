//
// Created by sexey on 28.02.2026.
//
module;
#include <windows.h>

#include "debug/debug_output.h"

export module sdk.locator.source2_engine_to_client;

import sdk.iface.scanner;

export namespace sdk
{
    void* GetSource2EngineToClient()
    {
        const auto instance = FindInterface(GetModuleHandleA("engine2.dll"), "Source2EngineToClient0");
        if (!instance) {
            dbg("Unable to find C_Source2EngineToClient instance!");
            return nullptr;
        }

        dbg("C_Source2EngineToClient ptr: {:p}", instance);
        return instance;
    }
}