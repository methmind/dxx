//
// Created by sexey on 22.12.2025.
//

#include "feature_fog_controller.h"

#include "hook/hook_dispatcher.h"
#include "hook/impl/hook_impl_type.h"
#include "sdk/datatype/sdk_fog_controller.h"
#include "sdk/util/sdk_game_state.h"
#include "service_locator/service_locator.h"
#include "sdk/custom/sdk_event_game_state.h"

namespace feature
{
    void C_FeatureFogController::onFireEvent(sdk::datatype::C_GameEvent* event) const
    {
        if (!sdk::custom::ValidateGameState(event, sdk::util::game_state_e::DOTA_GAMERULES_STATE_STRATEGY_TIME)) {
            return;
        }

        for (const auto& controllerList = this->eventList_->find(FOG_CONTROLLER_NAME);
            const auto& controller : controllerList) {
            auto& params = reinterpret_cast<sdk::datatype::C_FogController*>(controller)->getParams();
            params.getPlaneFarZ() = NEAR_FAR_PLANE_Z;
            params.isEnabled() = false;
        }
    }

    bool C_FeatureFogController::initialize() const
    {
        C_ServiceLocator::getInstance<hook::C_HookDispatcher>()->subscribe<void*, void*>(
            static_cast<hook::hook_id_t>(hook::impl::hook_impl_type_e::FIRE_EVENT),
            [this](void*, void* event){
                onFireEvent(static_cast<sdk::datatype::C_GameEvent*>(event));
            }
        );

        return true;
    }
} // feature