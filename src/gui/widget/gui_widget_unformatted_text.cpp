//
// Created by sexey on 23.10.2025.
//

#include "gui_widget_unformatted_text.h"

#include "imgui.h"

namespace gui::widget
{
    void C_WidgetUnformattedText::render()
    {
        if (!isVisible()) {
            return;
        }

        ImGui::TextUnformatted(this->label_.c_str());
    }
} // gui