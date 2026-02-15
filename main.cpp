#include <memory>
#include <windows.h>
#include <debug/debug_output.h>

import dxx;

DWORD WINAPI EntryModule(LPVOID lpModule)
{
    {
        MessageBoxA(nullptr, "Press F to pay respect", nullptr, 0);
        std::make_unique<dxx::C_Application>()->entry();
    }

    FreeLibraryAndExitThread(static_cast<HMODULE>(lpModule), 0);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    if (fdwReason != DLL_PROCESS_ATTACH) {
        return true;
    }

    if (!DisableThreadLibraryCalls(hinstDLL)) {
        dbg("DisableThreadLibraryCalls got:err = {}", GetLastError());
        return false;
    }

    const auto thread = CreateThread(nullptr, 0, EntryModule, hinstDLL, 0, nullptr);
    if (!thread) {
        dbg("CreateThread got:err = {}", GetLastError());
        return false;
    }

    CloseHandle(thread);
    return true;
}