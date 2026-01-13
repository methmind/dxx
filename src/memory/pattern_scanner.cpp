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
        const auto dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(module);
        const auto ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<uint8_t*>(module) + dosHeader->e_lfanew);

        return LightningScanner::Scanner(pattern).Find(module, ntHeaders->OptionalHeader.SizeOfImage).Get<void*>();
    }
} // memory