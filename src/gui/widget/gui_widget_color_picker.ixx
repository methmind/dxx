//
// Created by sexey on 01.03.2026.
//
module;
#include <format>
#include <string_view>

#include "nlohmann/json.hpp"
#include "imgui.h"

export module gui.widget.color_picker;

import gui.widget.base;

namespace gui
{
    export class C_WidgetColorPicker : public C_WidgetBase
    {
    public:
        ~C_WidgetColorPicker() override = default;

        explicit C_WidgetColorPicker(const std::string_view& id) :
            C_WidgetBase(std::format("##{}", id)), color_{0.0f, 0.0f, 0.0f, 255.0f} {}

        [[nodiscard]] ImVec4 getColor() const { return this->color_; }

        void setColor(const ImVec4& color) { this->color_ = color; }

        void render() override
        {
            if (!getVisibleState()) {
                return;
            }

            ImGui::ColorEdit4(getID().c_str(), reinterpret_cast<float*>(&this->color_),
                ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs
            );
        }

        void deserialize(const nlohmann::json& document) override
        {
            setColor({
                document.at("r").get<float>(),
                document.at("g").get<float>(),
                document.at("b").get<float>(),
                document.at("a").get<float>()
            });
        }

        void serialize(nlohmann::json& document) override
        {
            document["r"] = this->color_.x;
            document["g"] = this->color_.y;
            document["b"] = this->color_.z;
            document["a"] = this->color_.w;
        }

    private:
        ImVec4 color_;
    };
}