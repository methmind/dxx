//
// Created by sexey on 02.12.2025.
//

#ifndef DXX_DLC_SDK_BASE_MODEL_ENTITY_H
#define DXX_DLC_SDK_BASE_MODEL_ENTITY_H

#include "sdk_base_entity.h"
#include "sdk/util/sdk_color_t.h"

namespace sdk::datatype
{
    class C_BaseModelEntity : public C_BaseEntity
    {
    private:
        using on_color_changed_t = void(__fastcall*)(C_BaseModelEntity* self);

    public:
        SCHEMA_FIELD("C_BaseModelEntity", "m_clrRender", getRenderColor, util::color_t);

        void changeModelColor(util::color_t color);
    };
}

#endif //DXX_DLC_SDK_BASE_MODEL_ENTITY_H
