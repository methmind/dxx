//
// Created by sexey on 15.02.2026.
//
module;
#include <cstdint>

export module hook.type;

export namespace hook
{
    enum class hook_type_e : uint16_t
    {
        PRESENT = 0,
        WINDOW_MESSAGE,
        ON_PRE_IMGUI_RENDER,
        ON_IMGUI_RENDER,
        GET_MATRICES_FOR_VIEW,
    };
}
