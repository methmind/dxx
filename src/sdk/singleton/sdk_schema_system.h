//
// Created by sexey on 19.11.2025.
//

#ifndef DXX_DLC_SDK_SCHEMA_SYSTEM_SINGLETON_H
#define DXX_DLC_SDK_SCHEMA_SYSTEM_SINGLETON_H

#include <optional>
#include <string>
#include <unordered_map>

#include "memory/vmt_call.h"
#include "sdk/interface/sdk_schema_system.h"

namespace sdk::singleton
{
    /*
     * 48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 48 8B DA
     * void*(__fastcall*)(void* instance, const char* moduleName, void* unk);
    */
    constexpr auto FIND_TYPE_SCOPE_FOR_MODULE_VMT_INDEX = 13;

    class C_SchemaSystem
    {
    public:
        using schema_key_value_t = std::unordered_map<std::string, int16_t>;

        using schema_table_t = std::unordered_map<std::string, schema_key_value_t>;

    private:
        void* instance_;
        schema_table_t schemaTable_;

        [[nodiscard]] iface::C_SchemaTypeScope* findTypeScopeForModule(const char* moduleName) const
        {
            return memory::vmt::call<
                iface::C_SchemaTypeScope*(__fastcall*)(void*, const char*, void*),
                FIND_TYPE_SCOPE_FOR_MODULE_VMT_INDEX
            >(this->instance_, moduleName, nullptr);
        }

        bool initializeClassSchema(const std::string_view& className);

        schema_table_t::iterator getOrCreateSchemaTable(const std::string_view& className);

    public:

        std::optional<int16_t> getOffset(const std::string_view& className, const std::string_view& fieldName);

        bool initialize();

        C_SchemaSystem() : instance_(nullptr) {}

        ~C_SchemaSystem() = default;
    };
} // sdk

#endif //DXX_DLC_SDK_SCHEMA_SYSTEM_SINGLETON_H