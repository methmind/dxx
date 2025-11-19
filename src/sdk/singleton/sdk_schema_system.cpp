//
// Created by sexey on 19.11.2025.
//

#include "sdk_schema_system.h"

#include <array>
#include <span>

#include "debug/debug_output.h"
#include "sdk/interface/sdk_interface_scanner.h"

const char* MODULES_TO_SEARCH[] = { "client.dll" };

namespace sdk::singleton
{
    bool C_SchemaSystem::initializeClassSchema(const std::string_view& className)
    {
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

            auto [it, ok] = this->schemaTable_.emplace(className.data(), schema_key_value_t{});
            if (!ok) {
                dbg("Unable to add schema table for class: %s", className.data());
                return false;
            }

            for (std::span fields{ classInfo->getFields(), fieldsSize }; auto& field : fields) {
                it->second.emplace(field.name, field.offset);
            }

            return true;
        }

        return false;
    }

    C_SchemaSystem::schema_table_t::iterator C_SchemaSystem::getOrCreateSchemaTable(const std::string_view& className)
	{
		if (const auto& it = this->schemaTable_.find(className.data()); it != this->schemaTable_.end()) {
    		return it;
    	}

    	if (!initializeClassSchema(className)) {
    		dbg("Unable to find class schema: %s!", className.data());
    		return this->schemaTable_.end();
    	}

    	return this->schemaTable_.find(className.data());
    }

    std::optional<int16_t> C_SchemaSystem::getOffset(const std::string_view& className, const std::string_view& fieldName)
    {
        const auto& it = getOrCreateSchemaTable(className.data());
        if (it == this->schemaTable_.end()) {
            dbg("Unable to find class schema: %s!", className.data());
            return std::nullopt;
        }

        const auto& fieldInfo = it->second.find(fieldName.data());
        if (fieldInfo == it->second.end()) {
            dbg("Unable to find offset for field: %s::%s!", className.data(), fieldName.data());
            return std::nullopt;
        }

        dbg("[Schema] %s::%s: %hd", className.data(), fieldName.data(), fieldInfo->second);
        return fieldInfo->second;
    }

    bool C_SchemaSystem::initialize()
    {
        if (this->instance_ = iface::Find(GetModuleHandleA("schemasystem.dll"), "SchemaSystem_001"); !this->instance_) {
            dbg("Unable to find SchemaSystem instance!");
            return false;
        }

        dbg("C_SchemaSystem ptr: %p", this->instance_);
        return true;
    }
} // sdk