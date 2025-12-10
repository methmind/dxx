//
// Created by sexey on 07.11.2025.
//

#ifndef DXX_DLC_HOOK_MANAGER_H
#define DXX_DLC_HOOK_MANAGER_H

#include "MinHook.h"

namespace hook
{
    class C_HookManager
    {
    public:

        static MH_STATUS Create(void *target, void *detour);

        static bool Initialize();

        static bool Enable();

        static void Disable();
    };
} // hook

#endif //DXX_DLC_HOOK_MANAGER_H