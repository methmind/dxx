//
// Created by sexey on 01.11.2025.
//

#ifndef GUI_WIDGET_WINDOWS_CONTAINER_H
#define GUI_WIDGET_WINDOWS_CONTAINER_H

#include "gui/gui_container_interface.h"

namespace gui::widget
{
    class C_WidgetWindowsContainer : public C_IContainer
    {
    public:

        [[nodiscard]] widget_type_e getType() const override { return widget_type_e::WINDOWS_CONTAINER; }

        void render() override;

        void deserialize(const nlohmann::json& document) override {}

        void serialize(nlohmann::json& document) override {}

        explicit C_WidgetWindowsContainer(const std::string_view& id) : C_IWidget(id), C_IContainer(id) {}

        ~C_WidgetWindowsContainer() override = default;
    };
} // gui

#endif //GUI_WIDGET_WINDOWS_CONTAINER_H