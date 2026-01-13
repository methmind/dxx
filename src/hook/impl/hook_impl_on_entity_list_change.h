//
// Created by sexey on 23.11.2025.
//

#ifndef DXX_DLC_HOOK_IMPL_ON_ENTITY_LIST_CHANGE_H
#define DXX_DLC_HOOK_IMPL_ON_ENTITY_LIST_CHANGE_H

#include <cstdint>

namespace hook::impl
{
    void* __fastcall hkOnAddEntity(void* instance, void* entityInstance, int32_t handle);

    void* __fastcall hkOnRemoveEntity(void* instance, void* entityInstance, int32_t handle);
} // hook

#endif //DXX_DLC_HOOK_IMPL_ON_ENTITY_LIST_CHANGE_H