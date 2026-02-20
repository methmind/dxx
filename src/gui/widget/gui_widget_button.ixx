//
// Created by sexey on 19.02.2026.
//
module;
#include <format>
#include <string_view>

#include "imgui.h"

export module gui.widget.button;

import gui.widget.clickable;

namespace gui
{
    export class C_WidgetButton : public C_ClickableBase
    {
    public:
        ~C_WidgetButton() override = default;

        explicit C_WidgetButton(const std::string_view& id, const std::string_view& label) :
            C_ClickableBase(id), label_(std::format("{}##{}", label, id)), isAutoWidthX_(false) {}

        void setAutoWidthX(const bool autoWidth) { this->isAutoWidthX_ = autoWidth; }

        void render() override
        {
            if (!getVisibleState()) {
                return;
            }

            ImVec2 size{};
            if (this->isAutoWidthX_) {
                size.x = ImGui::GetContentRegionAvail().x;
                size.y = 0;
            }

            if (ImGui::Button(this->label_.c_str(), size)) {
                triggerOnClick();
            }
        }

    private:
        std::string label_;
        bool isAutoWidthX_;
    };
}