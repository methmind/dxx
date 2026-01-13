//
// Created by sexey on 02.12.2025.
//

#ifndef DXX_DLC_SDK_DOTA_BASE_NPC_H
#define DXX_DLC_SDK_DOTA_BASE_NPC_H

#include "sdk_base_model_entity.h"
#include "sdk_dota_modifier_manager.h"

namespace sdk::datatype
{
    class C_DotaBaseNPC : public C_BaseModelEntity
    {
    public:
        SCHEMA_FIELD("C_DOTA_BaseNPC", "m_iUnitType", getUnitType, uint32_t);
        SCHEMA_FIELD("C_DOTA_BaseNPC", "m_iIsControllableByPlayer64", isControllableByPlayer, uint64_t);
        SCHEMA_FIELD("C_DOTA_BaseNPC", "m_nPlayerOwnerID", getPlayerOwnerID, int32_t);
        SCHEMA_FIELD("C_DOTA_BaseNPC", "m_nUnitState64", getUnitState, uint64_t);
        SCHEMA_FIELD("C_DOTA_BaseNPC", "m_iHealthBarOffset", getHealthbarOffset, int32_t);
        SCHEMA_FIELD("C_DOTA_BaseNPC", "m_bHasClientSeenIllusionModifier", isSeenAsIllusion, bool);
        SCHEMA_FIELD("C_DOTA_BaseNPC", "m_bIsClone", isClone, bool);
        SCHEMA_FIELD("C_DOTA_BaseNPC", "m_ModifierManager", getModifierManager, C_DotaModifierManager);
        SCHEMA_FIELD("C_DOTA_BaseNPC", "m_iszUnitName", getUnitName, const char*);
    };

    class C_DotaBaseNPC_Hero : public C_DotaBaseNPC
    {
    public:
        SCHEMA_FIELD("C_DOTA_BaseNPC_Hero", "m_hReplicatingOtherHeroModel", getReplicatingHeroHandle, util::C_BaseEntityHandle);
    };
}

#endif //DXX_DLC_SDK_DOTA_BASE_NPC_H
