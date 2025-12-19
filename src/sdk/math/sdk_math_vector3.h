#ifndef DXX_DLC_SDK_MATH_VECTOR3_H
#define DXX_DLC_SDK_MATH_VECTOR3_H

namespace sdk::math
{
    struct vec3_s
    {
        float x{ 0.f }, y{ 0.f }, z{ 0.f };
    };

    using vector3 = vec3_s;
} // namespace sdk::math

#endif // DXX_DLC_SDK_MATH_VECTOR3_H