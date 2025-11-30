#include <memory>
#include <thread>

#include "application.h"
#include "debug/debug_output.h"

DWORD WINAPI UnloadModule(LPVOID lpModule)
{
    FreeLibraryAndExitThread(static_cast<HMODULE>(lpModule), 0);
}

DWORD WINAPI EntryModule(LPVOID lpModule)
{
    {
        std::make_unique<app::C_Application>()->entry();
    }

    CloseHandle(CreateThread(nullptr, 0, UnloadModule, lpModule, 0, nullptr));
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    if (fdwReason != DLL_PROCESS_ATTACH) {
        return true;
    }

    if (!DisableThreadLibraryCalls(hinstDLL)) {
        dbg("DisableThreadLibraryCalls got:err = %d", GetLastError());
        return false;
    }

    CloseHandle(CreateThread(nullptr, 0, EntryModule, hinstDLL, 0, nullptr));
    return true;
}