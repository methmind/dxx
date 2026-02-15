//
// Created by sexey on 14.02.2026.
//

#ifndef DXX_DLC_HOOK_INVOKER_H
#define DXX_DLC_HOOK_INVOKER_H
#include <cassert>
#include <type_traits>

#include "MinHook.h"

namespace hook
{
    class C_HookInvoker
    {
    private:
        FARPROC cached_;

    public:

        template<typename func_declaration_t, typename ... arg_t>
        __attribute__((always_inline)) auto invoke(arg_t ... args)
        {
            auto fn = reinterpret_cast<func_declaration_t>(this->cached_);

            if constexpr (std::is_void_v<func_declaration_t>) {
                fn(args...);
            } else {
                return fn(args...);
            }
        }

        explicit C_HookInvoker(void* detour) : cached_(FindTrampolineByDetour(detour))
        {
            assert(this->cached_ != nullptr && "FindTrampolineByDetour returned nullptr");
        }
    };

    #define MH_CALL_ORIGINAL(detour) []() -> auto& { \
        static hook::C_HookInvoker invoker(reinterpret_cast<void*>(detour)); \
        return invoker; \
    }().invoke<decltype(&detour)>
}

#endif //DXX_DLC_HOOK_INVOKER_H