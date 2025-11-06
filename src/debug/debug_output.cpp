#include "debug_output.h"
#include <format>
#include <iostream>
#include <sstream>
#include <windows.h>
#include <cstdarg>
#include <vector>
#include <memory>

void _dbg(const std::source_location& location, std::string_view format, ...)
{
    std::vector<char> buffer; buffer.resize(1024);

    va_list args;
    va_start(args, format);

    int needed_size = std::vsnprintf(buffer.data(), buffer.size(), format.data(), args);
    if (needed_size >= static_cast<int>(buffer.size())) {
        buffer.resize(needed_size + 1);
        va_end(args);
        va_start(args, format);
        std::vsnprintf(buffer.data(), buffer.size(), format.data(), args);
    }

    va_end(args);

    std::ostringstream oss;
    oss << "[" << location.file_name() << ":"
        << location.function_name() << ":"
        << location.line() << "]:\n" << buffer.data();

    OutputDebugStringA(oss.str().c_str());
}