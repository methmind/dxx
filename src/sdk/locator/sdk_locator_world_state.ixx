//
// Created by sexey on 28.02.2026.
//
module;
#include <windows.h>

#include "debug/debug_output.h"

export module sdk.locator.world_state;

import sdk.signature;

import memory.pattern_scanner;

export namespace sdk
{
    void* GetLevelInitCallback()
    {
        const auto callback = memory::FindPattern(GetModuleHandleA("client.dll"), ON_LEVEL_INIT_CALLBACK);
        if (!callback) {
            dbg("Unable to find level init callback!");
            return nullptr;
        }

        return callback;
    }

    void* GetLevelShutdownCallback()
    {
        const auto callback = memory::FindPattern(GetModuleHandleA("client.dll"), ON_LEVEL_SHUTDOWN_CALLBACK);
        if (!callback) {
            dbg("Unable to find level shutdown callback!");
            return nullptr;
        }

        return callback;
    }
}