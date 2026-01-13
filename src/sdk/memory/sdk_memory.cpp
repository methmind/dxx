//
// Created by sexey on 05.12.2025.
//

#include "sdk_memory.h"

#include <cassert>
#include <windows.h>

#define RESOLVE_TIER0(local_name, export_name) \
    static auto fn = reinterpret_cast<decltype(&local_name)>( \
    GetProcAddress(GetModuleHandleA("tier0.dll"), export_name)); \
    assert(fn && "Unable to find " export_name " function!");

namespace sdk::memory
{
    void* alloc(const size_t size)
    {
        RESOLVE_TIER0(alloc, "MemAlloc_AllocFunc");
        return fn(size);
    }

    void* realloc(void* ptr, const size_t size)
    {
        RESOLVE_TIER0(realloc, "MemAlloc_ReallocFunc");
        return fn(ptr, size);
    }

    size_t size(void* ptr)
    {
        RESOLVE_TIER0(size, "MemAlloc_GetSizeFunc");
        return fn(ptr);
    }

    void free(void* ptr)
    {
        RESOLVE_TIER0(free, "MemAlloc_FreeFunc");
        fn(ptr);
    }
} // sdk