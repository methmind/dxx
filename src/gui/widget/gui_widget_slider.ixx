//
// Created by sexey on 19.02.2026.
//
module;
#include <cstdint>

export module gui.widget.slider;

import gui.widget.slider_base;

export namespace gui
{
    using C_SliderInt = C_SliderBase<int32_t>;

    using C_SliderFloat = C_SliderBase<float>;
}