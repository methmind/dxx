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
        ON_IMGUI_RENDER,
        _internal_ON_RENDER_START,
        _internal_ON_RENDER_END,
        ON_RENDER_START,
    };
}
