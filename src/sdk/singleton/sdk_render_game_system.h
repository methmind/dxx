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
    constexpr auto WORLD_TO_PROJECTION_MATRIX_VMT_INDEX = 66;

    /*
     * 8B 05 ? ? ? ? 83 F8 ? 7E ? F7 05
     * float*(__fastcall*)(void* instance);
     */
    constexpr auto VIEW_MATRIX_VMT_INDEX = 65;

    class C_RenderGameSystem
    {
    private:
        using world_to_projection_matrix_t = float*(__fastcall*)(void* instance, int32_t screenSlot);
        using get_view_matrix_t = float*(__fastcall*)(void* instance);

        void* instance_;

    public:

        [[nodiscard]] float* getWorldProjectionMatrix() const;

        [[nodiscard]] float* getViewMatrix() const;

        bool initialize();

        C_RenderGameSystem() = default;

        ~C_RenderGameSystem() = default;
    };
} // sdk

#endif //DXX_DLC_SDK_RENDER_GAME_SYSTEM_H