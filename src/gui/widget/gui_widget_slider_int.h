//
// Created by sexey on 25.10.2025.
//

#ifndef GUI_WIDGET_SLIDER_INT_H
#define GUI_WIDGET_SLIDER_INT_H

#include "gui/gui_slider_interface.h"

namespace gui::widget
{
    class C_WidgetSliderInt : public C_ISlider<int32_t>
    {
    public:

        [[nodiscard]] widget_type_e getType() const override { return widget_type_e::SLIDER_INT; }

        explicit C_WidgetSliderInt(const std::string_view& id, const bool isVisible = true) :
            C_IWidget(id, isVisible), C_ISlider(id, isVisible) {}

        ~C_WidgetSliderInt() override = default;
    };
} // gui

#endif //GUI_WIDGET_SLIDER_INT_H