//
// Created by sexey on 04.03.2026.
//
module;
#include <windows.h>

#include "debug/debug_output.h"

export module sdk.locator.schema_system;

import sdk.iface.scanner;

export namespace sdk
{
    void* GetSchemaSystem()
    {
        const auto instance = FindInterface(GetModuleHandleA("schemasystem.dll"), "SchemaSystem_");
        if (!instance) {
            dbg("Unable to find C_SchemaSystem singleton!");
            return nullptr;
        }

        dbg("C_SchemaSystem instance: {:p}", instance);
        return instance;
    }
}