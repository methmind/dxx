//
// Created by sexey on 27.02.2026.
//
module;
#include <windows.h>

#include "debug/debug_output.h"

export module sdk.locator.panorama_ui_engine;

import sdk.iface.scanner;

export namespace sdk
{
    void* GetPanoramaUIEngine()
    {
        const auto instance = FindInterface(GetModuleHandleA("panorama.dll"), "PanoramaUIEngine0");
        if (!instance) {
            dbg("Unable to find C_PanoramaUIEngine instance!");
            return nullptr;
        }

        dbg("C_PanoramaUIEngine ptr: {:p}", instance);
        return instance;
    }
}