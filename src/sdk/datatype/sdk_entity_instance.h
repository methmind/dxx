//
// Created by sexey on 21.11.2025.
//

#ifndef DXX_DLC_SDK_ENTITY_INSTANCE_H
#define DXX_DLC_SDK_ENTITY_INSTANCE_H

#include "sdk_entity_identity.h"
#include "sdk/sdk_schema_field_proxy.h"

namespace sdk::datatype
{
    /*
     * 40 53 48 83 EC ? 48 8B DA E8 ? ? ? ? 48 8B 15 ? ? ? ? 48 8B CB E8 ? ? ? ? 48 8B C3 48 83 C4 ? 5B C3 CC CC CC CC CC CC CC CC CC CC 48 8B C4
     * void(__fastcall*)(void* instance, C_SchemaClassInfo* out);
     */
    constexpr auto GET_SCHEMA_CLASS_INFO_VMT_INDEX = 44;

    class C_EntityInstance
    {
    public:
        SCHEMA_FIELD("CEntityInstance", "m_pEntity", getIdentity, C_EntityIdentity*);

        iface::C_SchemaClassInfo* getClassInfo()
        {
            iface::C_SchemaClassInfo* classInfo = nullptr;
            memory::vmt::call<void(__fastcall*)(void* instance, void* out), GET_SCHEMA_CLASS_INFO_VMT_INDEX>(
                this, &classInfo
            );

            return classInfo;
        }
    };
}

#endif //DXX_DLC_SDK_ENTITY_INSTANCE_H