//
// Created by sexey on 10.12.2025.
//

#ifndef DXX_DLC_HOOK_IMPL_CREATE_MOVE_H
#define DXX_DLC_HOOK_IMPL_CREATE_MOVE_H

#include <cstdint>

namespace hook::impl
{
    void __fastcall hkCreateMove(void* instance, int32_t slot, bool isActive);
} // hook

#endif //DXX_DLC_HOOK_IMPL_CREATE_MOVE_H