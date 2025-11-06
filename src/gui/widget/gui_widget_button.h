//
// Created by sexey on 01.11.2025.
//

#ifndef GUI_WIDGET_BUTTON_H
#define GUI_WIDGET_BUTTON_H

#include <format>

#include "gui/gui_clickable_interface.h"

namespace gui::widget
{
    class C_WidgetButton : public C_IClickable
    {
    private:
        std::string label_;
        bool isWidthAuto_{false};

    public:

        [[nodiscard]] widget_type_e getType() const override { return widget_type_e::BUTTON; }

        void setWidthMode(const bool isAuto) { this->isWidthAuto_ = isAuto; }

        void render() override;

        explicit C_WidgetButton(const std::string_view& id, const std::string_view& label, const bool isVisible = true) :
            C_IWidget(id),
            C_IClickable(id, isVisible),
            label_(std::format("{}##{}", label, id)) {}

        ~C_WidgetButton() override = default;
    };
} // gui

#endif //GUI_WIDGET_BUTTON_H