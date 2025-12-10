//
// Created by sexey on 05.12.2025.
//

#ifndef DXX_DLC_SDK_DOTA_BUFF_H
#define DXX_DLC_SDK_DOTA_BUFF_H

#include "sdk/sdk_schema_field_proxy.h"
#include "sdk/util/sdk_base_entity_handle.h"

namespace sdk::datatype
{
    class C_DotaBuff
    {
    public:
        SCHEMA_FIELD("CDOTA_Buff", "m_name", getName, const char*);
        SCHEMA_FIELD("CDOTA_Buff", "m_iszModifierAura", getAuraName, const char*);
        SCHEMA_FIELD("CDOTA_Buff", "m_iSerialNumber", getSerialNumber, int32_t);
        SCHEMA_FIELD("CDOTA_Buff", "m_iIndex", getIndex, int32_t);
        SCHEMA_FIELD("CDOTA_Buff", "m_flCreationTime", getCreationTime, float);
        SCHEMA_FIELD("CDOTA_Buff", "m_flLastAppliedTime", getLastAppliedTime, float);
        SCHEMA_FIELD("CDOTA_Buff", "m_flDuration", getDuration, float);
        SCHEMA_FIELD("CDOTA_Buff", "m_flDieTime", getDieTime, float);
        SCHEMA_FIELD("CDOTA_Buff", "m_hCaster", getCasterHandle, util::C_BaseEntityHandle);
        SCHEMA_FIELD("CDOTA_Buff", "m_hAbility", getAbilityHandle, util::C_BaseEntityHandle);
        SCHEMA_FIELD("CDOTA_Buff", "m_hAuraOwner", getAuraOwner, util::C_BaseEntityHandle);
        SCHEMA_FIELD("CDOTA_Buff", "m_iStackCount", getStackCount, int32_t);
        SCHEMA_FIELD("CDOTA_Buff", "m_iAuraSearchTeam", getAuraSearchTeam, int16_t);
        SCHEMA_FIELD("CDOTA_Buff", "m_iAuraSearchType", getAuraSearchType, int16_t);
        SCHEMA_FIELD("CDOTA_Buff", "m_iAuraSearchFlags", getAuraSearchFlags, int32_t);
        SCHEMA_FIELD("CDOTA_Buff", "m_flAuraRadius", getAuraRadius, float);
        SCHEMA_FIELD("CDOTA_Buff", "m_iTeam", getTeam, int32_t);
        SCHEMA_FIELD("CDOTA_Buff", "m_iAttributes", getAttributes, int32_t);
        SCHEMA_FIELD("CDOTA_Buff", "m_bIsAura", isAura, bool);
        SCHEMA_FIELD("CDOTA_Buff", "m_bIsAuraActiveOnDeath", isAuraActiveOnDeath, bool);
        SCHEMA_FIELD("CDOTA_Buff", "m_bMarkedForDeletion", isMarkedForDeletion, bool);
        SCHEMA_FIELD("CDOTA_Buff", "m_bAuraIsHeal", isHealingAura, bool);
        SCHEMA_FIELD("CDOTA_Buff", "m_bProvidedByAura", isProvidedByAura, bool);
        SCHEMA_FIELD("CDOTA_Buff", "m_bCurrentlyInAuraRange", isInAuraRange, bool);
        SCHEMA_FIELD("CDOTA_Buff", "m_flThinkInterval", getThinkInterval, float);
    };
}

#endif //DXX_DLC_SDK_DOTA_BUFF_H