//
// Created by sexey on 19.11.2025.
//

#ifndef DXX_DLC_SDK_SCHEMA_SYSTEM_SINGLETON_H
#define DXX_DLC_SDK_SCHEMA_SYSTEM_SINGLETON_H

#include <optional>
#include <string>
#include <unordered_map>

#include "cs_plain_guarded.h"
#include "hash/xxhash_wrapper.h"
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
    private:
        using schema_key_value_t = std::unordered_map<std::string, uint16_t, xx_hashier_s, std::equal_to<>>;
        using schema_table_t = std::unordered_map<std::string, schema_key_value_t, xx_hashier_s, std::equal_to<>>;
        using guarded_schema_t = libguarded::plain_guarded<schema_table_t>;

        void* instance_;
        guarded_schema_t schemaTable_;

        [[nodiscard]] iface::C_SchemaTypeScope* findTypeScopeForModule(const char* moduleName) const
        {
            return ::memory::vmt::call<
                iface::C_SchemaTypeScope*(__fastcall*)(void*, const char*, void*),
                FIND_TYPE_SCOPE_FOR_MODULE_VMT_INDEX
            >(this->instance_, moduleName, nullptr);
        }

        std::optional<schema_key_value_t*> initializeClassSchema(const guarded_schema_t::handle& table,
            const std::string_view& className
        ) const;

        std::optional<schema_key_value_t*> getOrCreateSchemaTable(const guarded_schema_t::handle& table,
            const std::string_view& className
        ) const;

    public:

        std::optional<uint16_t> getOffset(const std::string_view& className, const std::string_view& fieldName);

        bool initialize();

        C_SchemaSystem() : instance_(nullptr) {}

        ~C_SchemaSystem() = default;
    };
} // sdk

#endif //DXX_DLC_SDK_SCHEMA_SYSTEM_SINGLETON_H