//
// Created by sexey on 24.02.2026.
//
module;
#include <cstdint>
#include <utility>

export module memory.vmt;

export namespace memory
{
    template <typename func_declaration_t, size_t id>
    __attribute__((always_inline)) decltype(auto) vcall(void* instance, auto ... args)
    {
        const auto vtable = *static_cast<void***>(instance);
        const auto vfn = reinterpret_cast<func_declaration_t>(vtable[id]);

        return vfn(instance, std::forward<decltype(args)>(args)...);
    }
}
