//
// Created by sexey on 22.02.2026.
//
module;
#include <cstdint>
#include <windows.h>
#include <string_view>

#include "debug/debug_output.h"
#include "minhook/src/hde/hde64.h"

export module sdk.iface.scanner;

namespace sdk
{
    struct interface_node_s
    {
        void* (__fastcall* create)();
        const char* name;
        interface_node_s* next;
    };

    interface_node_s* GetNodeList(HMODULE module)
    {
        const auto createInterfacePtr = reinterpret_cast<uint8_t*>(GetProcAddress(module, "CreateInterface"));
        if (!createInterfacePtr) {
            return nullptr;
        }

        hde64s hs;
        const auto length = hde64_disasm(createInterfacePtr, &hs);
        if (!length || hs.flags & F_ERROR) {
            dbg("Unable to disasm CreateInterface function!");
            return nullptr;
        }

        return *reinterpret_cast<interface_node_s**>(createInterfacePtr + hs.disp.disp32 + length);
    }

    export void* FindInterface(HMODULE module, const std::string_view& name)
    {
        const auto list = GetNodeList(module);
        if (!list) {
            dbg("Unable to get interface list!");
            return nullptr;
        }

        for (auto it = list; it != nullptr; it = it->next) {
            if (std::string_view(it->name).find(name) != std::string_view::npos) {
                return it->create();
            }
        }

        return nullptr;
    }
}
