//
// Created by sexey on 16.11.2025.
//

#include "hook_impl_frame_stage_notify.h"

#include "hook_impl_type.h"
#include "hook/hook_dispatcher.h"
#include "hook/hook_original_invoker.h"
#include "sdk/sdk_client_frame_stage_t.h"
#include "sdk/singleton/sdk_dota_view_render.h"
#include "sdk/singleton/sdk_source2_engine_to_client.h"
#include "service_locator/service_locator.h"

namespace hook::impl
{
    void __fastcall hkFrameStageNotify(void* self, int32_t frameStage)
    {
        MH_CALL_ORIGINAL(hkFrameStageNotify)(self, frameStage);

        const auto dispatcher = C_ServiceLocator::getInstance<C_HookDispatcher>();
        switch (frameStage) {
            case sdk::ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START:
                dispatcher->invoke(static_cast<hook_id_t>(hook_impl_type_e::ON_PRE_UPDATE));
                break;
            case sdk::ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_END: {
                dbg("Local player slot: %d, return value: %d",
                    C_ServiceLocator::getInstance<sdk::singleton::C_Source2EngineToClient>()->getLocalPlayerID()
                );
                dispatcher->invoke(static_cast<hook_id_t>(hook_impl_type_e::ON_UPDATE));
                break;
            }
            default: break;
        }
    }
} // hook