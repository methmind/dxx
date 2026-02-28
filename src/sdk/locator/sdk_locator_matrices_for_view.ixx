//
// Created by sexey on 28.02.2026.
//
module;
#include <windows.h>

#include "debug/debug_output.h"

export module sdk.locator.matrices_for_view;

import sdk.signature;
import memory.pattern_scanner;

export namespace sdk
{
    void* GetMatricesForView()
    {
        const auto func = memory::FindPattern(GetModuleHandleA("client.dll"), GET_MATRICES_FOR_VIEW);
        if (!func) {
            dbg("Unable to find pattern for GetMatricesForView");
            return nullptr;
        }

        return func;
    }
}
