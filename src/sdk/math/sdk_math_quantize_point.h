//
// Created by sexey on 14.12.2025.
//

#ifndef DXX_DLC_SDK_MATH_QUANTIZE_POINT_H
#define DXX_DLC_SDK_MATH_QUANTIZE_POINT_H

#include <algorithm>
#include <cstdint>
#include <cassert>
#include <windef.h>

#include "sdk_math_vector2.h"

namespace sdk::math
{
    constexpr auto QUANTIZED_WIDTH = 512.0f;
    constexpr auto QUANTIZED_HEIGHT = 384.0f;

    inline POINT QuantizePoint(const vector2& screenSize, const vector2& position)
    {
        assert(screenSize.x != 0 && screenSize.y != 0);

        const auto outX = static_cast<uint16_t>((position.x / screenSize.x) * QUANTIZED_WIDTH);
        const auto outY = static_cast<uint16_t>((position.y / screenSize.y) * QUANTIZED_HEIGHT);

        return {
            static_cast<uint16_t>(std::clamp<uint16_t>(outX, 0, static_cast<uint16_t>(QUANTIZED_WIDTH))),
            static_cast<uint16_t>(std::clamp<uint16_t>(outY, 0, static_cast<uint16_t>(QUANTIZED_HEIGHT)))
        };
    }
}

#endif //DXX_DLC_SDK_MATH_QUANTIZE_POINT_H