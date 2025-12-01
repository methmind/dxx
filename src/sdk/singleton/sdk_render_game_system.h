//
// Created by sexey on 01.12.2025.
//

#ifndef DXX_DLC_SDK_RENDER_GAME_SYSTEM_H
#define DXX_DLC_SDK_RENDER_GAME_SYSTEM_H

#include <cstdint>
#include "sdk/math/sdk_math_vector3.h"

namespace sdk::singleton
{
    constexpr auto RENDER_GAME_SYSTEM_SID = "RenderGameSystem";

    /*
     * 48 63 C2 48 8D 0D ? ? ? ? 48 C1 E0
     * float*(__fastcall*)(void* instance, int32_t screenSlot);
     */
    constexpr auto WORLD_TO_PROJECTION_MATRIX_VMT_INDEX = 67;

    class C_RenderGameSystem
    {
    private:
        using screen_transform_t = uint32_t(__fastcall*)(const math::Vector3& in, math::Vector3& out);

        using world_to_projection_matrix_t = float*(__fastcall*)(void* instance, int32_t screenSlot);

        void* instance_;
        screen_transform_t screenTransform_;

    public:

        [[nodiscard]] float* getWorldProjectionMatrix() const;

        uint32_t screenTransform(const math::Vector3& in, math::Vector3& out) const;

        bool initialize();

        C_RenderGameSystem() = default;

        ~C_RenderGameSystem() = default;
    };
} // sdk

#endif //DXX_DLC_SDK_RENDER_GAME_SYSTEM_H