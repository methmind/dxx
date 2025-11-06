//
// Created by sexey on 24.10.2025.
//

#ifndef GUI_WIDGET_SAME_LINE_H
#define GUI_WIDGET_SAME_LINE_H

#include "gui/gui_widget_interface.h"

namespace gui::widget
{
    class C_WidgetSameLine : public C_IWidget
    {
    public:

        [[nodiscard]] widget_type_e getType() const override { return widget_type_e::SAME_LINE; }

        void render() override;

        explicit C_WidgetSameLine(const std::string_view& id) : C_IWidget(id) {}

        ~C_WidgetSameLine() override = default;
    };
} // gui

#endif //GUI_WIDGET_SAME_LINE_H