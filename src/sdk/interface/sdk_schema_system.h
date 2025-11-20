//
// Created by sexey on 19.11.2025.
//

#ifndef DXX_DLC_SDK_SCHEMA_SYSTEM_H
#define DXX_DLC_SDK_SCHEMA_SYSTEM_H
#include "memory/vmt_call.h"

namespace sdk::iface
{
    struct schema_class_field_s
    {
        const char* name;
        uint8_t pad0[0x8];
        uint16_t offset;
        uint8_t pad1[0xE];
    };

    class C_SchemaClassInfo
    {
    public:

        const char* getName()
        {
            return *reinterpret_cast<const char**>(reinterpret_cast<uint8_t*>(this) + 0x8);
        }

        [[nodiscard]] uint16_t getFieldsSize() const
        {
            return *reinterpret_cast<const uint16_t*>(reinterpret_cast<const uint8_t*>(this) + 0x1C);
        }

        schema_class_field_s* getFields()
        {
            return *reinterpret_cast<schema_class_field_s**>(reinterpret_cast<uint8_t*>(this) + 0x28);
        }
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
            memory::vmt::call<void(__fastcall*)(void*, void*, const char*), FIND_DECLARED_CLASS_VMT_INDEX>(
                this, &res, className
            );

            return res;
        }
    };
}

#endif //DXX_DLC_SDK_SCHEMA_SYSTEM_H