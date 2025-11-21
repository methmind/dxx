//
// Created by sexey on 21.11.2025.
//

#ifndef DXX_DLC_SDK_BASE_HANDLE_H
#define DXX_DLC_SDK_BASE_HANDLE_H

#include <cstdint>

namespace sdk::util
{
    constexpr auto INVALID_ENTITY_HANDLE = 0xFFFFFFFF;

    constexpr auto ENTITY_ENTRY_MASK = 0x7FFF;

    constexpr auto SERIAL_NUMBER_SHIFT_BITS = 15;

    class C_BaseEntityHandle
    {
    private:
        uint32_t index_;

    public:

        [[nodiscard]] bool isValid() const
        {
            return this->index_ != INVALID_ENTITY_HANDLE;
        }

        [[nodiscard]] int32_t getEntryIndex() const
        {
            return this->index_ & ENTITY_ENTRY_MASK;
        }

        C_BaseEntityHandle() : index_(-1) {}

        explicit C_BaseEntityHandle(const int32_t index) : index_(index) {}

        explicit C_BaseEntityHandle(const int32_t index, const int32_t serial) : index_(index | (serial << SERIAL_NUMBER_SHIFT_BITS)) {}
    };
}

#endif //DXX_DLC_SDK_BASE_HANDLE_H