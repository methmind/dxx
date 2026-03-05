//
// Created by sexey on 04.03.2026.
//
module;
#include <cstdint>

#include "memory/offset.h"

export module sdk.schema.datatype;

import memory.vmt;

export namespace sdk
{
    struct schema_class_field_s
    {
        const char* name;
        uint8_t pad0[0x8];
        uint16_t offset;
        uint8_t pad1[0xE];
    };

    class C_SchemaClassInfo;

    struct schema_next_inheritance_s
    {
        uint8_t pad0[0x8];
        C_SchemaClassInfo* instance;
    };

    class C_SchemaClassInfo
    {
    public:
        OFFSET(const char*, getName, 0x8);
        OFFSET(uint32_t, getStructureSize, 0x18);
        OFFSET(uint16_t, getFieldsCount, 0x1C);
        OFFSET(uint32_t, getInheritanceDepth, 0x24);
        OFFSET(schema_class_field_s*, getFields, 0x28);
        OFFSET(schema_next_inheritance_s*, getNextInheritance, 0x30);
    };

    /*
     * 40 53 48 83 EC ? 48 8B 01 48 8B DA 49 8B D0 FF 90 ? ? ? ? 48 85 C0 74 ? 48 89 03 48 8B C3 48 83 C4 ? 5B C3 48 C7 03 ? ? ? ? 48 8B C3 48 83 C4 ? 5B C3 CC CC CC CC CC CC CC CC CC CC 40 53
     * void*(__fastcall*)(void* instance, C_SchemaClassInfo* out, const char* className);
     */
    constexpr auto FIND_DECLARED_CLASS_VMT_INDEX = 2;

    class C_SchemaTypeScope
    {
    public:

        C_SchemaClassInfo* findDeclaredClass(const char* className)
        {
            C_SchemaClassInfo* res = nullptr;
            memory::vcall<void(__attribute__((__fastcall__))*)(void*, void*, const char*), FIND_DECLARED_CLASS_VMT_INDEX>(
                this, &res, className
            );

            return res;
        }
    };
}
