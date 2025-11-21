//
// Created by sexey on 21.11.2025.
//

#ifndef DXX_DLC_SDK_SCHEMA_MACROS_H
#define DXX_DLC_SDK_SCHEMA_MACROS_H

#include <windows.h>
#include "service_locator/service_locator.h"
#include "singleton/sdk_schema_system.h"

#define SCHEMA_FIELD(className, fieldName, fieldVarName, type) \
    [[nodiscard]] std::add_lvalue_reference_t<type> fieldVarName() { \
        static auto cachedOffset{C_ServiceLocator::getInstance<sdk::singleton::C_SchemaSystem>()->getOffset(className, fieldName)}; \
        if (!cachedOffset.has_value()) {\
            MessageBoxA(nullptr, "Invalid schema offset (0) for field: " #className "::" #fieldName, nullptr, 0); \
            std::abort(); \
        }\
        return *std::launder(reinterpret_cast<type*>( \
            reinterpret_cast<uint8_t*>(this) + cachedOffset.value() \
        )); \
    }

#endif //DXX_DLC_SDK_SCHEMA_MACROS_H