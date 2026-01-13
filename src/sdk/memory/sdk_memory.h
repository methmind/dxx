//
// Created by sexey on 05.12.2025.
//

#ifndef DXX_DLC_SDK_MEMORY_H
#define DXX_DLC_SDK_MEMORY_H

#include <cstdint>

namespace sdk::memory
{
    void* alloc(size_t size);

    void* realloc(void* ptr, size_t size);

    size_t size(void* ptr);

    void free(void* ptr);
} // sdk

#endif //DXX_DLC_SDK_MEMORY_H