//
// Created by sexey on 24.12.2025.
//

#ifndef DXX_DLC_SDK_PHYS2_WORLD_H
#define DXX_DLC_SDK_PHYS2_WORLD_H

#include <windows.h>

#include "sdk/datatype/sdk_game_trace.h"
#include "sdk/datatype/sdk_trace_filter.h"
#include "sdk/math/sdk_math_vector3.h"

namespace sdk::singleton
{
    constexpr auto TRACE_MASK = 0x1C3003;

    class C_Phys2World
    {
    private:
        using trace_shape_t = bool(__fastcall*)(void* instance, datatype::ray_s* ray_t, math::vec3_s* start,
            math::vec3_s* end, datatype::trace_filter_s* filter, datatype::C_GameTrace* game_trace);

        void* instance_;
        trace_shape_t traceShapeFn_;

        bool findInstance(HMODULE clientModule);

        bool resolveFunctions(HMODULE clientModule);

    public:

        bool traceShape(datatype::ray_s* ray_t, math::vec3_s* start,
            math::vec3_s* end, datatype::trace_filter_s* filter, datatype::C_GameTrace* game_trace) const;

        bool initialize();

        C_Phys2World() = default;

        ~C_Phys2World() = default;
    };
} // sdk

#endif //DXX_DLC_SDK_PHYS2_WORLD_H