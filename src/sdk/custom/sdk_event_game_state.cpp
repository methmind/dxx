//
// Created by sexey on 24.12.2025.
//

#include "sdk_event_game_state.h"
#include "hash/xxhash_wrapper.h"

namespace sdk::custom
{
    bool ValidateGameState(datatype::C_GameEvent* event, util::game_state_e gameState)
    {
        if (!event || XXHASH_STR(event->getName()) != XXHASH_STR("dota_game_state_change")) {
            return false;
        }

        return event->getInt("new_state") == static_cast<int8_t>(gameState);
    }
} // sdk