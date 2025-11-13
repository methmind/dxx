//
// Created by sexey on 12.11.2025.
//

#ifndef DXX_DLC_SDK_INTERFACE_SCANNER_H
#define DXX_DLC_SDK_INTERFACE_SCANNER_H

#include <windows.h>
#include <string_view>

namespace sdk::iface
{
    struct interface_register_s
    {
        void* (__fastcall* create)();
        const char* name;
        interface_register_s* next;
    };

    void* Find(HMODULE module, const std::string_view& name);

    void PrintAll(HMODULE module);
} // sdk

#endif //DXX_DLC_SDK_INTERFACE_SCANNER_H