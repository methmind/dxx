//
// Created by sexey on 04.03.2026.
//
module;
#include <optional>
#include <ranges>
#include <unordered_map>

#include "cs_plain_guarded.h"
#include "debug/debug_output.h"

export module sdk.schema_system;

import xxhash.wrapper;
import memory.vmt;
import sdk.schema.datatype;

namespace sdk
{
    /*
     * 48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 48 8B DA
     * void*(__fastcall*)(void* instance, const char* moduleName, void* unk);
    */
    constexpr auto FIND_TYPE_SCOPE_FOR_MODULE_VMT_INDEX = 13;

    constexpr std::array<const char*, 1> MODULES_TO_SEARCH = { "client.dll" };

    using schema_key_value_t = std::unordered_map<std::string, uint16_t, xx_hasher_s, std::equal_to<>>;

    using schema_table_t = std::unordered_map<std::string, schema_key_value_t, xx_hasher_s, std::equal_to<>>;

    using guarded_schema_t = libguarded::plain_guarded<schema_table_t>;

    export class C_SchemaSystem
    {
    public:
        explicit C_SchemaSystem(void* instance) : instance_(instance) {}

        ~C_SchemaSystem() = default;

        [[nodiscard]] std::optional<uint16_t> getOffset(const std::string_view& className, const std::string_view& field)
        {
            const auto locked = this->table_.lock();

            auto* fieldMap = getOrCreateSchemaTable(locked, className);
            if (!fieldMap) {
                dbg("Unable to find class schema: {}!", className.data());
                return std::nullopt;
            }

            const auto it = fieldMap->find(field.data());
            if (it == fieldMap->end()) {
                dbg("Unable to find offset for field: {}::{}!", className.data(), field.data());
                return std::nullopt;
            }

            dbg("[Schema] {}::{}: {}", className.data(), field.data(), it->second);
            return it->second;
        }

    private:
        [[nodiscard]] C_SchemaTypeScope* findTypeScopeForModule(const char* moduleName) const
        {
            return memory::vcall<C_SchemaTypeScope*(__attribute__((__fastcall__))*)(void*, const char*, void*),
                FIND_TYPE_SCOPE_FOR_MODULE_VMT_INDEX
            >(this->instance_, moduleName, nullptr);
        }

        [[nodiscard]] C_SchemaClassInfo* findClassInModules(const std::string_view& className) const
        {
            for (const auto moduleName : MODULES_TO_SEARCH) {
                auto* typeScope = findTypeScopeForModule(moduleName);
                if (!typeScope) {
                    continue;
                }

                if (auto* classInfo = typeScope->findDeclaredClass(className.data())) {
                    return classInfo;
                }
            }

            return nullptr;
        }

        [[nodiscard]] schema_key_value_t* initializeClassSchema(const guarded_schema_t::handle& table, const std::string_view& className)
        {
            const auto* classInfo = findClassInModules(className);
            if (!classInfo) {
                return nullptr;
            }

            const auto fieldsSize = classInfo->getFieldsCount();
            if (!fieldsSize) {
                return nullptr;
            }

            auto [it, ok] = table->emplace(std::string{className}, schema_key_value_t{});
            if (!ok) {
                dbg("Unable to add schema table for class: {}", className.data());
                return nullptr;
            }

            it->second.reserve(fieldsSize);
            for (auto& field : std::views::counted(classInfo->getFields(), fieldsSize)) {
                it->second.emplace(field.name, field.offset);
            }

            return &it->second;
        }

        [[nodiscard]] schema_key_value_t* getOrCreateSchemaTable(const guarded_schema_t::handle& table, const std::string_view& className)
        {
            if (const auto it = table->find(className.data()); it != table->end()) {
                return &it->second;
            }

            if (auto* result = initializeClassSchema(table, className)) {
                return result;
            }

            dbg("Unable to initialize schema table for class: {}", className.data());
            return nullptr;
        }

        void* instance_;
        guarded_schema_t table_;
    };
}