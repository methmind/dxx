//
// Created by sexey on 04.03.2026.
//
module;
#include <cstdint>

export module sdk.handle;

namespace sdk
{
    constexpr auto INVALID_ENTITY_HANDLE = 0xFFFFFFFF;

    constexpr auto ENTITY_ENTRY_MASK = 0x7FFF;

    constexpr auto SERIAL_NUMBER_SHIFT_BITS = 15;

    export class C_BaseEntityHandle
    {
    private:
        int32_t index_;

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
