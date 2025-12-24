//
// Created by sexey on 24.12.2025.
//

#ifndef DXX_DLC_SDK_COLLISION_PROPERTY_H
#define DXX_DLC_SDK_COLLISION_PROPERTY_H

#include "sdk/sdk_schema_field_proxy.h"

namespace sdk::datatype
{
    class C_CollisionProperty
    {
    public:
        OFFSET(uint16_t, getCollisionMask, 0x38);
    };
}

#endif //DXX_DLC_SDK_COLLISION_PROPERTY_H