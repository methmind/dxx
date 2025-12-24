//
// Created by sexey on 24.12.2025.
//

#ifndef DXX_DLC_SDK_RAY_H
#define DXX_DLC_SDK_RAY_H

#include <cstdint>
#include "sdk/math/sdk_math_vector3.h"

namespace sdk::datatype
{
    enum class ray_type_e : uint8_t
    {
        RAY_TYPE_LINE = 0,
        RAY_TYPE_SPHERE,
        RAY_TYPE_HULL,
        RAY_TYPE_CAPSULE,
        RAY_TYPE_MESH,
    };

    struct ray_s
    {
        struct line_s
        {
            math::vector3 m_vStartOffset;
            float m_flRadius;
        };

        union
        {
            line_s m_Line;
            //https://github.com/Wend4r/sourcesdk/blob/main/public/gametrace.h
        };

        ray_type_e m_eType;

        explicit ray_s(const math::vector3& startOffset) : m_eType(ray_type_e::RAY_TYPE_LINE)
        {
            this->m_Line.m_vStartOffset = startOffset;
            this->m_Line.m_flRadius = 0.0f;
        }
    };
}

#endif //DXX_DLC_SDK_RAY_H