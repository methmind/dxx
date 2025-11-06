//
// Created by sexey on 22.10.2025.
//

#ifndef GUI_WIDGET_SEPARATOR_H
#define GUI_WIDGET_SEPARATOR_H

#include "gui/gui_widget_interface.h"

namespace gui::widget
{
    class C_WidgetSeparator final : public C_IWidget
    {
    private:
        std::string label_;

    public:

        [[nodiscard]] widget_type_e getType() const override { return widget_type_e::SEPARATOR; }

        void render() override;

        explicit C_WidgetSeparator(const std::string_view& id, const std::string_view& label) :
            C_IWidget(id), label_(label) {}

        ~C_WidgetSeparator() override = default;
    };
} // gui

#endif //GUI_WIDGET_SEPARATOR_H