//
// Created by sexey on 24.02.2026.
//
module;
#include <windows.h>

#include "debug/debug_output.h"

export module sdk.locator.vfile_system;

import sdk.iface.scanner;

export namespace sdk
{
    void* GetVFileSystem()
    {
        const auto instance = FindInterface(GetModuleHandleA("filesystem_stdio.dll"), "VFileSystem0");
        if (!instance) {
            dbg("Unable to find VFileSystem instance!");
            return nullptr;
        }

        dbg("VFileSystem ptr: {:p}", instance);
        return instance;
    }
}