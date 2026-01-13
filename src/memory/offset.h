//
// Created by sexey on 21.11.2025.
//

#ifndef DXX_DLC_OFFSET_H
#define DXX_DLC_OFFSET_H

#define OFFSET(type, fieldName, offset) \
    __forceinline std::add_lvalue_reference_t<type> fieldName() const { \
        return *reinterpret_cast<std::add_pointer_t<type>>(reinterpret_cast<uintptr_t>(this) + offset); \
    }

#endif //DXX_DLC_OFFSET_H