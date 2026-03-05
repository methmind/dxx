//
// Created by sexey on 04.03.2026.
//
module;
#include "sdk/schema/sdk_schema_system.h"

export module sdk.entity.instance;

import service.locator;
import memory.vmt;

import sdk.schema_system;
import sdk.entity.identity;
import sdk.schema.datatype;

namespace sdk
{
    /*
     * 40 53 48 83 EC ? 48 8B DA E8 ? ? ? ? 48 8B 15 ? ? ? ? 48 8B CB E8 ? ? ? ? 48 8B C3 48 83 C4 ? 5B C3 CC CC CC CC CC CC CC CC CC CC 48 8B C4
     * void(__fastcall*)(void* instance, C_SchemaClassInfo* out);
     */
    constexpr auto GET_SCHEMA_CLASS_INFO_VMT_INDEX = 44;

    export class C_EntityInstance
    {
    public:
        SCHEMA_FIELD("CEntityInstance", "m_pEntity", getIdentity, C_EntityIdentity*);

        [[nodiscard]] C_SchemaClassInfo* getClassInfo()
        {
            C_SchemaClassInfo* classInfo = nullptr;
            memory::vcall<void(__attribute__((__fastcall__))*)(void* instance, void* out), GET_SCHEMA_CLASS_INFO_VMT_INDEX>(
                this, &classInfo
            );

            return classInfo;
        }
    };
}