//
// Created by sexey on 11.11.2025.
//

#ifndef DXX_DLC_PATTERN_SCANNER_H
#define DXX_DLC_PATTERN_SCANNER_H

#include <string_view>
#include <windows.h>

namespace memory
{
    void* FindPattern(HMODULE module, const std::string_view& pattern);
} // memory

#endif //DXX_DLC_PATTERN_SCANNER_H