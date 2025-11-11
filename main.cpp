#include <thread>
#include "application.h"
#include "debug/debug_output.h"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    static auto core = std::make_unique<app::C_Application>();

    switch(fdwReason)
    {
        case DLL_PROCESS_ATTACH: {
            if (!DisableThreadLibraryCalls(hinstDLL)) {
                dbg("DisableThreadLibraryCalls got:err = %d", GetLastError());
                return false;
            }

            std::thread([hinstDLL]{ core->entry(hinstDLL); }).detach();
            break;
        }
        case DLL_PROCESS_DETACH: {
            core->dispose();
            break;
        }
        default: { break; }
    }

    return TRUE;
}
