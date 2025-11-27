#include <memory>
#include <thread>

#include "application.h"
#include "debug/debug_output.h"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    if (fdwReason != DLL_PROCESS_ATTACH) {
        return TRUE;
    }

    if (!DisableThreadLibraryCalls(hinstDLL)) {
        dbg("DisableThreadLibraryCalls got:err = %d", GetLastError());
        return false;
    }

    std::thread([hinstDLL] {
        {
            std::make_unique<app::C_Application>()->entry();
        }
        FreeLibraryAndExitThread(hinstDLL, 0);
    }).detach();

    return TRUE;
}