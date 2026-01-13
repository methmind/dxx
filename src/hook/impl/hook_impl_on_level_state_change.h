//
// Created by sexey on 22.11.2025.
//

#ifndef DXX_DLC_HOOK_IMPL_ON_LEVEL_STATE_CHANGE_H
#define DXX_DLC_HOOK_IMPL_ON_LEVEL_STATE_CHANGE_H

namespace hook::impl
{
    void* __fastcall hkOnLevelInit(void* instance, const char* mapName);

    void* __fastcall hkOnLevelShutdown(void* instance);
} // hook

#endif //DXX_DLC_HOOK_IMPL_ON_LEVEL_STATE_CHANGE_H