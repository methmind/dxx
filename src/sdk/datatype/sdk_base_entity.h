//
// Created by sexey on 21.11.2025.
//

#ifndef DXX_DLC_SDK_BASE_ENTITY_H
#define DXX_DLC_SDK_BASE_ENTITY_H

#include "sdk_collision_property.h"
#include "sdk_entity_instance.h"

namespace sdk::datatype
{
    class C_BaseEntity : public C_EntityInstance
    {
    public:
        SCHEMA_FIELD("C_BaseEntity", "m_iMaxHealth", getMaxHealth, int32_t);
        SCHEMA_FIELD("C_BaseEntity", "m_iHealth", getHealth, int32_t);
        SCHEMA_FIELD("C_BaseEntity", "m_fFlags", getEntityFlags, int32_t);
        SCHEMA_FIELD("C_BaseEntity", "m_iTeamNum", getTeam, int32_t);
        SCHEMA_FIELD("C_BaseEntity", "m_flSimulationTime", getSimulationTime, float);
        SCHEMA_FIELD("C_BaseEntity", "m_pCollision", getCollisionComponent, C_CollisionProperty*);
    };
}

#endif //DXX_DLC_SDK_BASE_ENTITY_H