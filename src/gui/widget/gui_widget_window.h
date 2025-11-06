//
// Created by sexey on 21.10.2025.
//

#ifndef GUI_WIDGET_WINDOW_H
#define GUI_WIDGET_WINDOW_H

#include <format>

#include "imgui.h"
#include "gui/gui_container_interface.h"

namespace gui::widget
{
    class C_WidgetWindow : public C_IContainer
    {
    private:
        std::string title_;

        ImVec2 pos_;
        ImVec2 size_;
        bool isRectInvalidated_;

    public:

        [[nodiscard]] widget_type_e getType() const override { return widget_type_e::WINDOW; }

        [[nodiscard]] const ImVec2& getPosition() const { return this->pos_; }

        [[nodiscard]] const ImVec2& getSize() const { return this->size_; }

        void setPosition(const ImVec2& pos) {
            this->pos_ = pos;
            this->isRectInvalidated_ = true;
        }

        void setSize(const ImVec2& size) {
            this->size_ = size;
            this->isRectInvalidated_ = true;
        }

        void render() override;

        void deserialize(const nlohmann::json& document) override {}

        void serialize(nlohmann::json& document) override {}

        explicit C_WidgetWindow(const std::string_view& id, const std::string_view& title, const bool isVisible = true) :
            C_IWidget(id, isVisible),
            C_IContainer(id, isVisible),
            title_(std::format("{}##{}", title, id)),
            isRectInvalidated_(false) {}

        ~C_WidgetWindow() override = default;
    };
} // gui

#endif //GUI_WIDGET_WINDOW_H