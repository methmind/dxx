//
// Created by sexey on 17.02.2026.
//
module;
#include <cstring>
#include <string>
#include <type_traits>

#include "xxhash.h"

export module xxhash.wrapper;

export struct xx_hasher_s
{
    using is_transparent = void;

    template<typename T>
    uint64_t operator()(const T& str) const
    requires (std::is_same_v<T, std::string> || std::is_same_v<T, std::string_view>)
    {
        return XXH3_64bits(str.data(), str.size());
    }

    uint64_t operator()(const char* str) const
    {
        return str ? XXH3_64bits(str, strlen(str)) : 0;
    }

    static constexpr uint64_t cx_hash(const std::string_view& str)
    {
        return XXH3_64bits(str.data(), str.size());
    }
};