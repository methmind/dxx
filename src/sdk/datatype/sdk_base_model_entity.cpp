//
// Created by sexey on 02.12.2025.
//

#include "sdk_base_model_entity.h"
#include <cassert>

#include "../../memory/offset_manager.h"

namespace sdk::datatype
{
    void C_BaseModelEntity::changeModelColor(const util::color_t color)
    {
        static auto fn{reinterpret_cast<on_color_changed_t>(memory::OffsetManager::GetBaseModelEntityChangeColor())};
        assert(fn && "Unable to find C_BaseModelEntity::OnColorChanged method!");

        this->getRenderColor() = color;
        fn(this);
    }
}
