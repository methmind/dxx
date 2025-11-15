//
// Created by sexey on 15.11.2025.
//

#ifndef DXX_DLC_HOOK_IMPL_TYPE_H
#define DXX_DLC_HOOK_IMPL_TYPE_H

#include <cstdint>

namespace hook::impl
{
    enum class hook_impl_type_e : uint16_t
    {
        UNKNOWN = 0,
        PRESENT,
        ON_RENDER_START,
        ON_LUA_DISPOSE,
    };
}

#endif //DXX_DLC_HOOK_IMPL_TYPE_H