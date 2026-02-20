//
// Created by sexey on 18.02.2026.
//
module;
#include <string_view>

#include "imgui.h"

export module gui.widget.same_line;

import gui.widget.base;

namespace gui
{
    export class C_WidgetSameLine : public C_WidgetBase
    {
    public:
        explicit C_WidgetSameLine(const std::string_view& id) : C_WidgetBase(id) {}

        void render() override
        {
            if (!getVisibleState()) {
                return;
            }

            ImGui::SameLine();
        }
    };
}