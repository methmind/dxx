//
// Created by sexey on 24.12.2025.
//

#ifndef DXX_DLC_SDK_EVENT_GAME_STATE_H
#define DXX_DLC_SDK_EVENT_GAME_STATE_H

#include "sdk/datatype/sdk_game_event.h"
#include "sdk/util/sdk_game_state.h"

namespace sdk::custom
{
    bool ValidateGameState(datatype::C_GameEvent* event, util::game_state_e gameState);
} // sdk

#endif //DXX_DLC_SDK_EVENT_GAME_STATE_H