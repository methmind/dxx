//
// Created by sexey on 24.10.2025.
//

#include "gui_widget_same_line.h"

#include "imgui.h"

namespace gui::widget
{
    void C_WidgetSameLine::render()
    {
        if (!isVisible()) {
            return;
        }

        ImGui::SameLine();
    }
} // gui