//
// Created by sexey on 07.11.2025.
//

#ifndef DXX_DLC_HOOK_ORIGINAL_INVOKER_H
#define DXX_DLC_HOOK_ORIGINAL_INVOKER_H

#include <cassert>
#include <type_traits>
#include "MinHook.h"

namespace hook
{
    class C_HookOriginalInvoker
    {
    private:
        FARPROC cached_;

    public:

        template<typename func_declaration_t, typename ... arg_t>
        __forceinline auto invoke(arg_t ... args)
        {
            auto fn = reinterpret_cast<func_declaration_t>(this->cached_);

            if constexpr (std::is_void_v<func_declaration_t>) {
                fn(args...);
            } else {
                return fn(args...);
            }
        }

        explicit C_HookOriginalInvoker(void* detour) : cached_(FindTrampolineByDetour(detour))
        {
            assert(this->cached_ != nullptr && "FindTrampolineByDetour returned nullptr");
        }
    };

    #define MH_CALL_ORIGINAL(detour) []() -> auto& { \
        static C_HookOriginalInvoker invoker(reinterpret_cast<void*>(detour)); \
        return invoker; \
    }().invoke<decltype(&detour)>
}

#endif //DXX_DLC_HOOK_ORIGINAL_INVOKER_H