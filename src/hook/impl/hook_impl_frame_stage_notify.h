//
// Created by sexey on 16.11.2025.
//

#ifndef DXX_DLC_HOOK_IMPL_FRAME_STAGE_NOTIFY_H
#define DXX_DLC_HOOK_IMPL_FRAME_STAGE_NOTIFY_H

#include <cstdint>

namespace hook::impl
{
    void __fastcall hkFrameStageNotify(void* self, int32_t frameStage);
} // hook

#endif //DXX_DLC_HOOK_IMPL_FRAME_STAGE_NOTIFY_H