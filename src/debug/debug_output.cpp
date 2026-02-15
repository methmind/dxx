//
// Created by sexey on 13.02.2026.
//

#include "debug_output.h"

#include <format>
#include <string>
#include <windows.h>

void dbg_impl(const std::source_location& location, const std::string_view fmt, const std::format_args args)
{
    std::string message = std::vformat(fmt, args);

    OutputDebugStringA(std::format("[{}:{}]:\n{}",
        location.function_name(),
        location.line(),
        message
    ).c_str());
}