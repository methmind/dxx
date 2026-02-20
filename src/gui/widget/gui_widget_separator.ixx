//
// Created by sexey on 18.02.2026.
//
module;
#include <string>

#include "imgui.h"

export module gui.widget.separator;

import gui.widget.base;

namespace gui
{
    export class C_WidgetSeparator final : public C_WidgetBase
    {
    public:
        ~C_WidgetSeparator() override = default;

        explicit C_WidgetSeparator(const std::string_view& id, const std::string_view& label) :
            C_WidgetBase(id), label_(label) {}

        void render() override
        {
            if (!getVisibleState()) {
                return;
            }

            ImGui::SeparatorText(this->label_.c_str());
        }

    private:
        std::string label_;
    };
}