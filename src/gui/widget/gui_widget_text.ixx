//
// Created by sexey on 19.02.2026.
//
module;
#include <string_view>

#include "imgui.h"

export module gui.widget.text;

import gui.widget.base;

namespace gui
{
    export class C_WidgetText : public C_WidgetBase
    {
    public:
        ~C_WidgetText() override = default;

        explicit C_WidgetText(const std::string_view& id, const std::string_view& label) :
            C_WidgetBase(id), label_(label) {}

        void render() override
        {
            if (!getVisibleState()) {
                return;
            }

            ImGui::TextUnformatted(this->label_.c_str());
        }

    private:
        std::string label_;
    };
}