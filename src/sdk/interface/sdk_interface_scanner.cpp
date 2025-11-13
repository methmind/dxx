//
// Created by sexey on 12.11.2025.
//

#include "sdk_interface_scanner.h"

#include <cstdint>
#include "debug/debug_output.h"
#include "minhook/src/hde/hde64.h"

namespace sdk::iface
{
    interface_register_s* GetInterfaceList(HMODULE module)
    {
        const auto createInterfacePtr = reinterpret_cast<uint8_t*>(GetProcAddress(module, "CreateInterface"));
        if (!createInterfacePtr) {
            dbg("Unable to get CreateInterface address!");
            return nullptr;
        }

        hde64s hs;
        const auto length = hde64_disasm(createInterfacePtr, &hs);
        if (!length || hs.flags & F_ERROR) {
            dbg("Unable to disasm CreateInterface function!");
            return nullptr;
        }

        return *reinterpret_cast<interface_register_s**>(createInterfacePtr + hs.disp.disp32 + length);
    }

    void* Find(HMODULE module, const std::string_view& name)
    {
        const auto list = GetInterfaceList(module);
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

    void PrintAll(HMODULE module)
    {
        const auto list = GetInterfaceList(module);
        if (!list) {
            dbg("Unable to get interface list!");
        }

        for (auto it = list; it != nullptr; it = it->next) {
            dbg("Interface: %s", it->name);
        }
    }
} // sdk