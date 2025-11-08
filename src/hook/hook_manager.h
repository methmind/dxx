//
// Created by sexey on 07.11.2025.
//

#ifndef DXX_DLC_HOOK_MANAGER_H
#define DXX_DLC_HOOK_MANAGER_H

namespace hook
{
    class C_HookManager
    {
    private:

    public:

        bool initialize();

        bool enable();

        C_HookManager() = default;

        ~C_HookManager();
    };
} // hook

#endif //DXX_DLC_HOOK_MANAGER_H