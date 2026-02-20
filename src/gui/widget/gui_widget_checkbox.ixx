//
// Created by sexey on 19.02.2026.
//
module;
#include <format>
#include <string_view>

#include "nlohmann/json.hpp"
#include "imgui.h"

export module gui.widget.checkbox;

import gui.widget.clickable;

namespace gui
{
    export class C_WidgetCheckbox : public C_ClickableBase
    {
    public:
        ~C_WidgetCheckbox() override = default;

        explicit C_WidgetCheckbox(const std::string_view& id, const std::string_view& label) :
            C_ClickableBase(id), label_(std::format("{}##{}", label, id)), checked_(false) {}

        [[nodiscard]] int32_t getState() const override { return this->checked_; }

        void setState(const int32_t state) override
        {
            this->checked_ = state;
            triggerOnClick();
        }

        void render() override
        {
            if (!getVisibleState()) {
                return;
            }

            if (ImGui::Checkbox(this->label_.c_str(), &this->checked_)) {
                triggerOnClick();
            }
        }

        void deserialize(const nlohmann::json& document) override
        {
            setState(document.at("state").get<bool>());
        }

        void serialize(nlohmann::json& document) override
        {
            document["state"] = this->checked_;
        }

    private:
        std::string label_;
        bool checked_;
    };
}