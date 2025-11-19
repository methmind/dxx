//
// Created by sexey on 16.11.2025.
//

#include "sdk_source2_engine_to_client.h"

#include "debug/debug_output.h"
#include "sdk/interface/sdk_interface_scanner.h"
#include "memory/vmt_call.h"

namespace sdk::singleton
{
    bool C_Source2EngineToClient::isInGame() const
    {
        return memory::vmt::call<bool>(this->instance_, IS_IN_GAME_VMT_INDEX);
    }

    uint32_t C_Source2EngineToClient::getLocalPlayerID() const
    {
        int32_t playerSlot = -1;
        memory::vmt::call<void>(this->instance_, GET_LOCAL_PLAYER_VMT_INDEX, &playerSlot, 0);

        return playerSlot + 1;
    }

    bool C_Source2EngineToClient::initialize()
    {
        if (this->instance_ = iface::Find(GetModuleHandleA("engine2.dll"), "Source2EngineToClient0"); !this->instance_) {
            dbg("Unable to find Source2EngineToClient instance!");
            return false;
        }

        dbg("C_Source2EngineToClient ptr: %p", this->instance_);
        return true;
    }
} // sdk