//
// Created by sexey on 04.03.2026.
//

#ifndef DXX_DLC_SDK_SCHEMA_SYSTEM_H
#define DXX_DLC_SDK_SCHEMA_SYSTEM_H

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <type_traits>

/*
 * @brief Requires the calling translation unit to have:
 *  import service.locator;
 *  import sdk.schema_system;
 */
#define SCHEMA_FIELD(className, fieldName, fieldVarName, type) \
    [[nodiscard]] __attribute__((always_inline)) std::add_lvalue_reference_t<type> fieldVarName() \
    { \
        static const auto cachedOffset = C_ServiceLocator::Get<sdk::C_SchemaSystem>()->getOffset(className, fieldName); \
        if (!cachedOffset.has_value()) { \
            assert(false && "Invalid schema offset for field: " className "::" fieldName); \
        } \
        return *reinterpret_cast<std::add_pointer_t<type>>(reinterpret_cast<uint8_t*>(this) + cachedOffset.value()); \
    }

#endif //DXX_DLC_SDK_SCHEMA_SYSTEM_H