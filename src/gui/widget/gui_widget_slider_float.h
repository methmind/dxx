//
// Created by sexey on 03.11.2025.
//

#ifndef GUI_WIDGET_SLIDER_FLOAT_H
#define GUI_WIDGET_SLIDER_FLOAT_H

#include "gui/gui_slider_interface.h"

namespace gui::widget
{
    class C_WidgetSliderFloat : public C_ISlider<float>
    {
    public:

        [[nodiscard]] widget_type_e getType() const override { return widget_type_e::SLIDER_FLOAT; }

        explicit C_WidgetSliderFloat(const std::string_view& id, const bool isVisible = true) :
            C_IWidget(id, isVisible), C_ISlider(id, isVisible) {}

        ~C_WidgetSliderFloat() override = default;
    };
} // gui

#endif //GUI_WIDGET_SLIDER_FLOAT_H
