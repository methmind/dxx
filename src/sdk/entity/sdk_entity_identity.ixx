//
// Created by sexey on 04.03.2026.
//
module;
#include "memory/offset.h"
#include "sdk/schema/sdk_schema_system.h"

export module sdk.entity.identity;

import service.locator;
import sdk.schema_system;

export namespace sdk
{
    class C_EntityIdentity
    {
    public:
        OFFSET(void*, getAssignedEntity, 0x0);
        OFFSET(int32_t, getEntityHandle, 0x10);
        SCHEMA_FIELD("CEntityIdentity", "m_name", getName, const char*);
        SCHEMA_FIELD("CEntityIdentity", "m_designerName", getDesignerName, const char*);
        SCHEMA_FIELD("CEntityIdentity", "m_flags", getFlags, uint32_t);
    };
}