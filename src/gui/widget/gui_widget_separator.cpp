//
// Created by sexey on 22.10.2025.
//

#include "gui_widget_separator.h"
#include "imgui.h"

namespace gui::widget
{
    void C_WidgetSeparator::render()
    {
        if (!isVisible()) {
            return;
        }

        ImGui::SeparatorText(this->label_.c_str());
    }
} // gui