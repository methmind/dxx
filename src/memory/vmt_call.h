//
// Created by sexey on 17.11.2025.
//

#ifndef DXX_DLC_VMT_CALL_H
#define DXX_DLC_VMT_CALL_H

#include <cstdint>
#include <type_traits>
#include <utility>
#include <_mingw.h>

namespace memory::vmt
{
    template <typename return_t, typename ... arg_t>
    __forceinline return_t call(void* instance, const size_t index, arg_t&& ... args)
    {
        const auto vtable = *static_cast<void***>(instance);

        using vfn_t = return_t (__thiscall*)(void* instance, arg_t&& ... args);
        const auto vfn = reinterpret_cast<vfn_t>(vtable[index]);

        if constexpr (std::is_void_v<return_t>) {
            vfn(instance, std::forward<arg_t>(args)...);
        } else {
            return vfn(instance, std::forward<arg_t>(args)...);
        }

        return return_t();
    }
}

#endif //DXX_DLC_VMT_CALL_H