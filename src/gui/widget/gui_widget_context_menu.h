//
// Created by sexey on 22.10.2025.
//

#ifndef GUI_WIDGET_CONTEXT_MENU_H
#define GUI_WIDGET_CONTEXT_MENU_H

#include <format>

#include "gui/gui_container_interface.h"

namespace gui::widget
{
    class C_WidgetContextMenu : public C_IContainer
    {
    private:
        std::string label_;

    public:

        [[nodiscard]] widget_type_e getType() const override { return widget_type_e::CONTEXT_MENU; }

        void render() override;

        explicit C_WidgetContextMenu(const std::string_view& id, const std::string_view& label) :
            C_IWidget(id), C_IContainer(id), label_(std::format("{}##{}", label, id)) {}

        ~C_WidgetContextMenu() override = default;
    };
} // gui

#endif //GUI_WIDGET_CONTEXT_MENU_H