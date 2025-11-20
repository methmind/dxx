//
// Created by sexey on 19.11.2025.
//

#include "sdk_schema_system.h"

#include <array>
#include <ranges>
#include <span>

#include "debug/debug_output.h"
#include "sdk/interface/sdk_interface_scanner.h"

constexpr std::array<const char*, 1> MODULES_TO_SEARCH = { "client.dll" };

namespace sdk::singleton
{
    std::optional<C_SchemaSystem::schema_key_value_t*> C_SchemaSystem::initializeClassSchema(
        const guarded_schema_t::handle& table,
        const std::string_view& className
    ) const {
        // todo Decompose this...
        for (const auto moduleName : MODULES_TO_SEARCH) {
            const auto typeScope = findTypeScopeForModule(moduleName);
            if (!typeScope) {
                continue;
            }

            const auto classInfo = typeScope->findDeclaredClass(className.data());
            if (!classInfo) {
                continue;
            }

            const auto fieldsSize = classInfo->getFieldsSize();
            if (!fieldsSize) {
                continue;
            }

            auto [it, ok] = table->emplace(className.data(), schema_key_value_t{});
            if (!ok) {
                dbg("Unable to add schema table for class: %s", className.data());
                return std::nullopt;
            }

            for (auto& field : std::views::counted(classInfo->getFields(), fieldsSize)) {
                it->second.emplace(field.name, field.offset);
            }

            return &it->second;
        }

        return std::nullopt;
    }

    std::optional<C_SchemaSystem::schema_key_value_t*> C_SchemaSystem::getOrCreateSchemaTable(
        const guarded_schema_t::handle& table,
        const std::string_view& className
    ) const {
        if (const auto& it = table->find(className.data()); it != table->end()) {
            return &it->second;
        }

        if (const auto it = initializeClassSchema(table, className); it.has_value()) {
            return it;
        }

        dbg("Unable to initialize schema table for class: %s", className.data());
        return std::nullopt;
    }

    std::optional<uint16_t> C_SchemaSystem::getOffset(const std::string_view& className, const std::string_view& fieldName)
    {
        const auto locked = this->schemaTable_.lock();

        const auto table = getOrCreateSchemaTable(locked, className);
        if (!table.has_value()) {
            dbg("Unable to find class schema: %s!", className.data());
            return std::nullopt;
        }

        const auto& fieldInfo = table.value()->find(fieldName.data());
        if (fieldInfo == table.value()->end()) {
            dbg("Unable to find offset for field: %s::%s!", className.data(), fieldName.data());
            return std::nullopt;
        }

        dbg("[Schema] %s::%s: %hd", className.data(), fieldName.data(), fieldInfo->second);
        return fieldInfo->second;
    }

    bool C_SchemaSystem::initialize()
    {
        if (this->instance_ = iface::Find(GetModuleHandleA("schemasystem.dll"), "SchemaSystem_"); !this->instance_) {
            dbg("Unable to find SchemaSystem instance!");
            return false;
        }

        dbg("C_SchemaSystem ptr: %p", this->instance_);
        return true;
    }
} // sdk