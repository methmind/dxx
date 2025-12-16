//
// Created by sexey on 11.11.2025.
//

#include "pattern_scanner.h"

#include "debug/debug_output.h"
#include "LightningScanner/LightningScanner.hpp"

namespace memory
{
    void* FindPattern(HMODULE module, const std::string_view& pattern)
    {
        if (!module || pattern.empty()) {
            return nullptr;
        }

        auto* base = reinterpret_cast<uint8_t*>(module);
        const auto* dos = reinterpret_cast<PIMAGE_DOS_HEADER>(base);
        if (dos->e_magic != IMAGE_DOS_SIGNATURE) {
            return nullptr;
        }

        auto* nt = reinterpret_cast<PIMAGE_NT_HEADERS>(base + dos->e_lfanew);
        if (nt->Signature != IMAGE_NT_SIGNATURE) {
            return nullptr;
        }

        const auto* textSection = IMAGE_FIRST_SECTION(nt);
        auto* textStart = base + textSection->VirtualAddress;
        auto textSize = static_cast<size_t>(textSection->Misc.VirtualSize);
        if (textSize == 0) {
            textSize = static_cast<size_t>(textSection->SizeOfRawData);
        }

        return LightningScanner::Scanner(pattern).Find(textStart, textSize).Get<void*>();
    }
} // memory