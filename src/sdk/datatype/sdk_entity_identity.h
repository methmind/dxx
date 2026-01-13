//
// Created by sexey on 21.11.2025.
//

#ifndef DXX_DLC_SDK_ENTITY_IDENTITY_H
#define DXX_DLC_SDK_ENTITY_IDENTITY_H

#include "memory/offset.h"
#include "sdk/sdk_schema_field_proxy.h"

namespace sdk::datatype
{
    class C_EntityIdentity
    {
    private:
        uint8_t size_[0x70]{};

    public:
        OFFSET(void*, getAssignedEntity, 0x0);
        OFFSET(int32_t, getEntityHandle, 0x10);
        SCHEMA_FIELD("CEntityIdentity", "m_name", getName, const char*);
        SCHEMA_FIELD("CEntityIdentity", "m_designerName", getDesignerName, const char*);
        SCHEMA_FIELD("CEntityIdentity", "m_flags", getFlags, uint32_t);
    };
}

#endif //DXX_DLC_SDK_ENTITY_IDENTITY_H