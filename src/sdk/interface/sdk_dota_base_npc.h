//
// Created by sexey on 02.12.2025.
//

#ifndef DXX_DLC_SDK_DOTA_BASE_NPC_H
#define DXX_DLC_SDK_DOTA_BASE_NPC_H

#include "sdk_base_model_entity.h"

namespace sdk::iface
{
    class C_DotaBaseNPC : public C_BaseModelEntity
    {
    public:
        SCHEMA_FIELD("C_DOTA_BaseNPC", "m_iHealthBarOffset", getHealthbarOffset, int32_t);
    };

    class C_DotaBaseNPC_Hero : public C_DotaBaseNPC
    {
    public:
        SCHEMA_FIELD("C_DOTA_BaseNPC_Hero", "m_hReplicatingOtherHeroModel", getReplicatingHeroHandle, util::C_BaseEntityHandle);
    };
}

#endif //DXX_DLC_SDK_DOTA_BASE_NPC_H
