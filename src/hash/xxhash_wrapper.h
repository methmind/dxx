//
// Created by sexey on 19.11.2025.
//

#ifndef DXX_DLC_XXHASH_WRAPPER_H
#define DXX_DLC_XXHASH_WRAPPER_H

#include <string>
#include "xxhash3/xxhash.h"

struct xx_hashier_s
{
    uint64_t operator()(const std::string& str) const
    {
        return XXH64(str.data(), str.size(), 0);
    }
};

#endif //DXX_DLC_XXHASH_WRAPPER_H