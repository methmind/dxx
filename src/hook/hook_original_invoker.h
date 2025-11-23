//
// Created by sexey on 07.11.2025.
//

#ifndef DXX_DLC_HOOK_ORIGINAL_INVOKER_H
#define DXX_DLC_HOOK_ORIGINAL_INVOKER_H

#include <cassert>
#include "MinHook.h"

namespace hook
{
    class C_HookOriginalInvoker
    {
    private:
        void* detour_;
        mutable FARPROC cached_;

    public:

        template<typename func_declaration_t, typename ... arg_t>
        __forceinline auto invoke(arg_t ... args)
        {
            return reinterpret_cast<func_declaration_t>(this->cached_)(args...);
        }

        constexpr explicit C_HookOriginalInvoker(void* detour) : detour_(detour), cached_(FindTrampolineByDetour(detour))
        {
            assert(this->cached_ != nullptr && "FindTrampolineByDetour returned nullptr");
        }
    };

    #define MH_CALL_ORIGINAL(detour) C_HookOriginalInvoker(reinterpret_cast<void*>(detour)).invoke<decltype(&detour)>
}

#endif //DXX_DLC_HOOK_ORIGINAL_INVOKER_H