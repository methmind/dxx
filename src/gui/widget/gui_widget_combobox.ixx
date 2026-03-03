//
// Created by sexey on 02.03.2026.
//
module;
#include <format>
#include <string_view>

#include "nlohmann/json.hpp"
#include "imgui.h"

export module gui.widget.combobox;

import gui.widget.base;

namespace gui
{
    export class C_WidgetCombobox : public C_WidgetBase
    {
    public:
        ~C_WidgetCombobox() override = default;

        explicit C_WidgetCombobox(const std::string_view& id) :
            C_WidgetBase(std::format("##{}", id)), selectedIndex_(0) {}

        void setOptions(const std::vector<std::string>& options) { this->options_ = options; }

        void setSelectedIndex(int32_t selectedIndex)
        {
            if (selectedIndex < 0 || selectedIndex >= static_cast<int32_t>(this->options_.size())) {
                return;
            }

            this->selectedIndex_ = selectedIndex;
        }

        [[nodiscard]] int32_t getSelectedIndex() const { return this->selectedIndex_; }

        void render() override
        {
            if (!getVisibleState())  {
                return;
            }

            const auto* preview = "...";
            if (this->selectedIndex_ >= 0 && this->selectedIndex_ < static_cast<int32_t>(this->options_.size())) {
                preview = this->options_[this->selectedIndex_].c_str();
            }

            ImGui::PushItemWidth(-1.0f);
            if (ImGui::BeginCombo(getID().c_str(), preview)) {
                for (auto i = 0; i < static_cast<int32_t>(this->options_.size()); i++) {
                    const auto isSelected = (i == this->selectedIndex_);
                    if (ImGui::Selectable(this->options_[i].c_str(), isSelected)) {
                        this->selectedIndex_ = i;
                    }

                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::PopItemWidth();
        }

        void deserialize(const nlohmann::json& document) override
        {
            setSelectedIndex(document.at("value").get<int32_t>());
        }

        void serialize(nlohmann::json& document) override
        {
            document["value"] = this->selectedIndex_;
        }

    private:
        int32_t selectedIndex_;
        std::vector<std::string> options_;
    };
}