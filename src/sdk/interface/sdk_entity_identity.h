//
// Created by sexey on 21.11.2025.
//

#ifndef DXX_DLC_SDK_ENTITY_IDENTITY_H
#define DXX_DLC_SDK_ENTITY_IDENTITY_H

#include "memory/offset.h"
#include "sdk/sdk_schema_field_proxy.h"
#include "sdk/util/sdk_base_entity_handle.h"

namespace sdk::iface
{
    class C_EntityIdentity
    {
    public:
        OFFSET(int32_t, getIndex, 0x10);
        SCHEMA_FIELD("CEntityIdentity", "m_name", getName, const char*);
        SCHEMA_FIELD("CEntityIdentity", "m_designerName", getDesignerName, const char*);
        SCHEMA_FIELD("CEntityIdentity", "m_flags", getFlags, uint32_t);

        [[nodiscard]] bool isValid() const
        {
            return util::C_BaseEntityHandle(getIndex()).isValid();
        }

        bool isSameType(const char* typeName)
        {
            return strcmp(getDesignerName(), typeName) == 0;
        }
    };
}

#endif //DXX_DLC_SDK_ENTITY_IDENTITY_H