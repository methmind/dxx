//
// Created by sexey on 22.12.2025.
//

#ifndef DXX_DLC_SDK_FOG_CONTROLLER_H
#define DXX_DLC_SDK_FOG_CONTROLLER_H

#include "sdk_base_entity.h"
#include "sdk_fog_params.h"

namespace sdk::datatype
{
    class C_FogController : public C_BaseEntity
    {
    public:
        SCHEMA_FIELD("C_FogController", "m_fog", getParams, fog_params_t_s);
    };
}

#endif //DXX_DLC_SDK_FOG_CONTROLLER_H