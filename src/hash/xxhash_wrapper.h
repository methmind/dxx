//
// Created by sexey on 19.11.2025.
//

#ifndef DXX_DLC_XXHASH_WRAPPER_H
#define DXX_DLC_XXHASH_WRAPPER_H

#include <string>
#include <cstring>
#include "xxhash3/xxhash.h"
#include "xxh64.hpp"

struct xx_hashier_s
{
    using is_transparent = void;

    template<typename T>
    uint64_t operator()(const T& str) const
    requires (std::is_same_v<T, std::string> || std::is_same_v<T, std::string_view>)
    {
        return XXH64(str.data(), str.size(), 0);
    }

    uint64_t operator()(const char* str) const
    {
        return str ? XXH64(str, strlen(str), 0) : 0;
    }

    static constexpr uint64_t cx_hash(const std::string_view& str)
    {
        return xxh64::hash(str.data(), str.size(), 0);
    }
};

#define XXHASH_STR(str) xx_hashier_s::cx_hash(str)

#endif //DXX_DLC_XXHASH_WRAPPER_H