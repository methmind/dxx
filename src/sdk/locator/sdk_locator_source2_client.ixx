//
// Created by sexey on 22.02.2026.
//
module;
#include <windows.h>

#include "debug/debug_output.h"

export module sdk.locator.source2_client;

import sdk.iface.scanner;

export namespace sdk
{
    void* GetSource2Client()
    {
        const auto instance = FindInterface(GetModuleHandleA("client.dll"), "Source2Client0");
        if (!instance) {
            dbg("Unable to find Source2Client instance!");
            return nullptr;
        }

        dbg("C_Source2Client ptr: {:p}", instance);
        return instance;
    }
}