#ifndef DEBUG_OUTPUT_H
#define DEBUG_OUTPUT_H

#include <string_view>
#include <source_location>

#ifndef DEBUG_OUTPUT
#define dbg(format, ...)
#else
#define dbg(format, ...) _dbg(std::source_location::current(), format, ##__VA_ARGS__)
void _dbg(const std::source_location& location, std::string_view format, ...);
#endif

#endif //DEBUG_OUTPUT_H