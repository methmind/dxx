//
// Created by sexey on 01.12.2025.
//

#include "sdk_render_game_system.h"

#include "sdk_base_game_system_factory.h"
#include "debug/debug_output.h"
#include "../../memory/offset_manager.h"
#include "memory/vmt_call.h"
#include "service_locator/service_locator.h"

namespace sdk::singleton
{
    float* C_RenderGameSystem::getWorldProjectionMatrix() const
    {
        return memory::vmt::call<world_to_projection_matrix_t, WORLD_TO_PROJECTION_MATRIX_VMT_INDEX>(*static_cast<void**>(this->instance_), 0);
    }

    uint32_t C_RenderGameSystem::screenTransform(const math::vector3& in, math::vector3& out) const
    {
        return this->screenTransform_(in, out);
    }

    bool C_RenderGameSystem::initialize()
    {
        if (this->instance_ = C_ServiceLocator::getInstance<C_BaseGameSystemFactory>()->find(RENDER_GAME_SYSTEM_SID);
            !this->instance_) {
            dbg("Unable to find C_RenderGameSystem!");
            return false;
        }

        if (this->screenTransform_ = reinterpret_cast<screen_transform_t>(
            memory::OffsetManager::GetScreenTransform());
            !this->screenTransform_) {
            dbg("Unable to find ScreenTransform function!");
            return false;
        }

        dbg("C_RenderGameSystem ptr: %p", this->instance_);
        return true;
    }
} // sdk