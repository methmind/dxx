//
// Created by sexey on 18.02.2026.
//
module;
#include <string_view>

#include "imgui.h"

export module gui.widget.menu_item;

import gui.widget.clickable;

namespace gui
{
    export class C_WidgetMenuItem : public C_ClickableBase
    {
    public:
        ~C_WidgetMenuItem() override = default;

        explicit C_WidgetMenuItem(const std::string_view& id, const std::string_view& label) :
            C_ClickableBase(id), label_(label) {}

        void render() override
        {
            if (!getVisibleState()) {
                return;
            }

            if (ImGui::MenuItem(this->label_.c_str())) {
                triggerOnClick();
            }
        }

    private:
        std::string label_;
    };
}