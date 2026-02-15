//
// Created by sexey on 13.02.2026.
//

#ifndef DXX_DLC_DEBUG_OUTPUT_H
#define DXX_DLC_DEBUG_OUTPUT_H

#ifdef DEBUG_OUTPUT
    #include <format>
    #include <source_location>
    #include <string_view>

    struct debug_format_s
    {
        std::string_view fmt;
        std::source_location loc;

        template <typename T>
        consteval debug_format_s(const T& s, const std::source_location l = std::source_location::current()) : fmt(s), loc(l) {}
    };

    void dbg_impl(const std::source_location& location, std::string_view fmt, std::format_args args);

    template<typename ... args_t>
    __attribute__((always_inline)) void _dbg(const debug_format_s format_obj, args_t&& ... args)
    {
        dbg_impl(
            format_obj.loc,
            format_obj.fmt,
            std::make_format_args(args...)
        );
    }

    #define dbg(format, ...) _dbg(format, ##__VA_ARGS__)
#else
    #define dbg(...)
#endif


#endif //DXX_DLC_DEBUG_OUTPUT_H