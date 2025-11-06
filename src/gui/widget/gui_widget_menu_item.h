//
// Created by sexey on 22.10.2025.
//

#ifndef GUI_WIDGET_MENU_ITEM_H
#define GUI_WIDGET_MENU_ITEM_H

#include <format>

#include "gui/gui_clickable_interface.h"

namespace gui::widget
{
    class C_WidgetMenuItem : public C_IClickable
    {
    private:
        std::string label_;

    public:

        [[nodiscard]] widget_type_e getType() const override { return widget_type_e::MENU_ITEM; }

        void render() override;

        explicit C_WidgetMenuItem(const std::string_view& id, const std::string_view& label, const bool isVisible = true) :
            C_IWidget(id), C_IClickable(id, isVisible), label_(std::format("{}##{}", label, id)) {}

        ~C_WidgetMenuItem() override = default;
    };
} // gui

#endif //GUI_WIDGET_MENU_ITEM_H
