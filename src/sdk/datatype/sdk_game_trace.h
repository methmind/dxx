//
// Created by sexey on 24.12.2025.
//

#ifndef DXX_DLC_SDK_GAME_TRACE_H
#define DXX_DLC_SDK_GAME_TRACE_H

#include <cstdint>

#include "sdk_ray.h"
#include "sdk/math/sdk_math_vector3.h"

namespace sdk::datatype
{
    class C_GameTrace
    {
    public:
        void* m_pSurface; // 0x00
        void* m_pHitEntity; // 0x08
        void* m_pHitboxData; // 0x10
        uint8_t pad02[0x38]; // 0x18
        uint32_t m_uContents; // 0x50
        uint8_t pad03[0x24]; // 0x54
        math::vector3 m_vecStartPos; // 0x78
        math::vector3 m_vecEndPos; // 0x84
        math::vector3 m_vecNormal; // 0x90
        math::vector3 m_vecPosition; // 0x9C
        uint8_t pad04[0x4]; // 0xA8
        float m_flFraction; // 0xAC
        uint8_t pad05[0x5];
        ray_type_e m_eRayType;
        bool m_bAllSolid;
        uint8_t pad06[0x4D]; // 0xB7
    };
}

#endif //DXX_DLC_SDK_GAME_TRACE_H