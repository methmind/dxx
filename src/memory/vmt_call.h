//
// Created by sexey on 17.11.2025.
//

#ifndef DXX_DLC_VMT_CALL_H
#define DXX_DLC_VMT_CALL_H

#include <cstdint>
#include <type_traits>
#include <utility>

namespace memory::vmt
{
    template <typename func_declaration_t, size_t id>
    __forceinline auto call(void* instance, auto ... args)
    {
        const auto vtable = *static_cast<void***>(instance);
        const auto vfn = reinterpret_cast<func_declaration_t>(vtable[id]);

        if constexpr (std::is_void_v<func_declaration_t>) {
            vfn(instance, std::forward<decltype(args)>(args)...);
        } else {
            return vfn(instance, std::forward<decltype(args)>(args)...);
        }
    }
}

#endif //DXX_DLC_VMT_CALL_H