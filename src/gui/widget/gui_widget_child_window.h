//
// Created by sexey on 23.10.2025.
//

#ifndef GUI_WIDGET_CHILD_WINDOW_H
#define GUI_WIDGET_CHILD_WINDOW_H

#include "imgui.h"
#include "gui/gui_container_interface.h"
#include "gui/gui_widget_interface.h"

namespace gui::widget
{
    class C_WidgetChildWindow : public C_IContainer
    {
    private:
        ImVec2 size_;
        ImGuiWindowFlags flags_;
        ImGuiChildFlags childFlags_;

    public:

        [[nodiscard]] widget_type_e getType() const override { return widget_type_e::CHILD_WINDOW; }

        void setSize(const ImVec2& size)
        {
            this->size_ = size;
        }

        void render() override;

        void deserialize(const nlohmann::json& document) override {}

        void serialize(nlohmann::json& document) override {}

        explicit C_WidgetChildWindow(
            const std::string_view& id,
            const ImGuiWindowFlags flags = ImGuiWindowFlags_None,
            const ImGuiChildFlags childFlags = ImGuiChildFlags_Borders
        ) : C_IWidget(id), C_IContainer(id), flags_(flags), childFlags_(childFlags) {}

        ~C_WidgetChildWindow() override = default;
    };
} // gui

#endif //GUI_WIDGET_CHILD_WINDOW_H
