//
// Created by sexey on 22.12.2025.
//

#ifndef DXX_DLC_FEATURE_FOG_CONTROLLER_H
#define DXX_DLC_FEATURE_FOG_CONTROLLER_H

#include "sdk/custom/sdk_entity_list.h"
#include "sdk/datatype/sdk_entity_instance.h"
#include "sdk/datatype/sdk_game_event.h"

namespace feature
{
    constexpr auto NEAR_FAR_PLANE_Z = 999999.f;

    constexpr std::string FOG_CONTROLLER_NAME = "C_FogController";

    class C_FeatureFogController
    {
    private:
        std::shared_ptr<sdk::custom::C_EntityList> eventList_;

        void onFireEvent(sdk::datatype::C_GameEvent* event) const;

    public:

        [[nodiscard]] bool initialize() const;

        explicit C_FeatureFogController(const std::shared_ptr<sdk::custom::C_EntityList>& eventList) :
            eventList_(eventList) {}

        ~C_FeatureFogController() = default;
    };
} // feature

#endif //DXX_DLC_FEATURE_FOG_CONTROLLER_H