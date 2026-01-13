//
// Created by sexey on 21.11.2025.
//

#ifndef DXX_DLC_SDK_DOTA_PLAYER_CONTROLLER_H
#define DXX_DLC_SDK_DOTA_PLAYER_CONTROLLER_H

#include "sdk_base_entity.h"
#include "sdk/util/sdk_base_entity_handle.h"

namespace sdk::datatype
{
    class C_DotaPlayerController : public C_BaseEntity
    {
    public:
        SCHEMA_FIELD("CBasePlayerController", "m_iszPlayerName", getPlayerName, char[128]);
        SCHEMA_FIELD("CBasePlayerController", "m_steamID", getSteamID, uint64_t);
        //
        SCHEMA_FIELD("C_DOTAPlayerController", "m_hAssignedHero", getAssignedHeroHandle, util::C_BaseEntityHandle);
    };
}

#endif //DXX_DLC_SDK_DOTA_PLAYER_CONTROLLER_H