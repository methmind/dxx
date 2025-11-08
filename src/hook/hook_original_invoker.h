//
// Created by sexey on 07.11.2025.
//

#ifndef DXX_DLC_HOOK_ORIGINAL_INVOKER_H
#define DXX_DLC_HOOK_ORIGINAL_INVOKER_H

#include <cassert>
#include <utility>
#include "MinHook.h"

namespace hook
{
    class C_HookOriginalInvoker
    {
    private:
        void* detour_;

    public:

        template<typename return_t, typename ... arg_t>
        __forceinline decltype(auto) invoke(arg_t&& ... args)
        {
            static auto original{FindTrampolineByDetour(this->detour_)};
            assert(original != nullptr && "FindTrampolineByDetour returned nullptr");

            return reinterpret_cast<return_t>(original)(std::forward<arg_t>(args)...);
        }

        constexpr explicit C_HookOriginalInvoker(void* detour) : detour_(detour) {}
    };

    #define MH_CALL_ORIGINAL(detour) C_HookOriginalInvoker(reinterpret_cast<void*>(detour)).invoke<decltype(&detour)>
}

#endif //DXX_DLC_HOOK_ORIGINAL_INVOKER_H