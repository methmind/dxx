//
// Created by sexey on 25.10.2025.
//

#ifndef GUI_WIDGET_CHECKBOX_H
#define GUI_WIDGET_CHECKBOX_H

#include <format>

#include "gui/gui_clickable_interface.h"

namespace gui::widget
{
    class C_WidgetCheckbox : public C_IClickable
    {
    private:
        std::string label_;
        bool checked_;

    public:

        void setState(int32_t state) override;

        [[nodiscard]] widget_type_e getType() const override { return widget_type_e::CHECKBOX; }

        [[nodiscard]] int32_t getState() const override { return this->checked_; }

        void render() override;

        void deserialize(const nlohmann::json &document) override;

        void serialize(nlohmann::json& document) override;

        explicit C_WidgetCheckbox(const std::string_view& id, const std::string_view& label, const bool isVisible = true) :
            C_IWidget(id), C_IClickable(id, isVisible), label_(std::format("{}##{}", label, id)), checked_(false) {}

        ~C_WidgetCheckbox() override = default;
    };
} // gui

#endif //GUI_WIDGET_CHECKBOX_H