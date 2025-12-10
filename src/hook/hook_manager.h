//
// Created by sexey on 07.11.2025.
//

#ifndef DXX_DLC_HOOK_MANAGER_H
#define DXX_DLC_HOOK_MANAGER_H

#include <windows.h>

namespace hook
{
    class C_HookManager
    {
    private:

        bool initializeRender();

        bool initializeWorld(HMODULE clientModule);

        bool initializeEntity();

    public:

        bool initialize();

        bool enable();

        // Костыли-костыли... Тут либо нужно настраивать пайплайн вызова деструкторов в C_ServiceContainer, либо так.
        void disable();

        C_HookManager() = default;

        ~C_HookManager() = default;
    };
} // hook

#endif //DXX_DLC_HOOK_MANAGER_H