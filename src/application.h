//
// Created by sexey on 06.11.2025.
//

#ifndef DXX_DLC_CORE_H
#define DXX_DLC_CORE_H

#include <windows.h>

namespace app
{
    class C_Application
    {
    private:
        HANDLE disposeEvent_{};

    public:

        bool entry();

        C_Application() = default;

        ~C_Application() { CloseHandle(this->disposeEvent_); }
    };
} // core

#endif //DXX_DLC_CORE_H