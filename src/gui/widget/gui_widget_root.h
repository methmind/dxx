//
// Created by sexey on 28.10.2025.
//

#ifndef GUI_WIDGET_ROOT_H
#define GUI_WIDGET_ROOT_H

#include "gui/gui_container_interface.h"
#include "gui/gui_widget_interface.h"

namespace gui::widget
{
    constexpr auto ROOT_WIDGET_ID = "root";

    class C_WidgetRoot : public C_IContainer
    {
    public:

        void render() override;

        explicit C_WidgetRoot() : C_IWidget(ROOT_WIDGET_ID), C_IContainer(ROOT_WIDGET_ID) {}

        ~C_WidgetRoot() override = default;
    };
} // gui

#endif //GUI_WIDGET_ROOT_H