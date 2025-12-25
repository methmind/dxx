//
// Created by sexey on 22.12.2025.
//

#ifndef DXX_DLC_SDK_FOG_PARAMS_H
#define DXX_DLC_SDK_FOG_PARAMS_H

#include "sdk/sdk_schema_field_proxy.h"

namespace sdk::datatype
{
    struct fog_params_t_s
    {
        SCHEMA_FIELD("fogparams_t", "farz", getPlaneFarZ, float);
        SCHEMA_FIELD("fogparams_t", "enable", isEnabled, bool);
    };
}

#endif //DXX_DLC_SDK_FOG_PARAMS_H