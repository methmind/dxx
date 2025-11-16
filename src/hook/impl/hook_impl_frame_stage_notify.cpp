//
// Created by sexey on 16.11.2025.
//

#include "hook_impl_frame_stage_notify.h"

#include "hook_impl_type.h"
#include "hook/hook_dispatcher.h"
#include "hook/hook_original_invoker.h"
#include "sdk/sdk_client_frame_stage_t.h"
#include "service_locator/service_locator.h"

namespace hook::impl
{
    void hkFrameStageNotify(void* self, int32_t frameStage)
    {
        MH_CALL_ORIGINAL(hkFrameStageNotify)(self, frameStage);

        const auto dispatcher = C_ServiceLocator::getInstance<C_HookDispatcher>();
        switch (frameStage) {
            //case sdk::ClientFrameStage_t::NET_UPDATE_START:
            //    dispatcher->invoke(static_cast<hook_id_t>(hook_impl_type_e::ON_PRE_UPDATE));
            //    break;
            case sdk::ClientFrameStage_t::NET_UPDATE_END:
                dispatcher->invoke(static_cast<hook_id_t>(hook_impl_type_e::ON_UPDATE));
                break;
            default: break;
        }
    }
} // hook