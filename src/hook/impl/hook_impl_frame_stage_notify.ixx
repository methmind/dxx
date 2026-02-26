//
// Created by sexey on 22.02.2026.
//
module;
#include "hook/hook_invoker.h"

export module hook.impl.frame_stage_notify;

import service.locator;

import hook.dispatcher;
import hook.type;

import sdk.datatype.client_frame_stage_t;

export namespace hook
{
    void __attribute__((__fastcall__)) hkFrameStageNotify(void* self, const sdk::ClientFrameStage_t frameStage)
    {
        MH_CALL_ORIGINAL(hkFrameStageNotify)(self, frameStage);
    }
}
