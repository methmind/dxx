//
// Created by sexey on 01.11.2025.
//

#include "gui_widget_button.h"

#include "imgui.h"

namespace gui::widget
{
    void C_WidgetButton::render()
    {
        if (!isVisible()) {
            return;
        }

        ImVec2 size{};
        if (this->isWidthAuto_) {
            size.x = ImGui::GetContentRegionAvail().x;
            size.y = 0;
        }

        if (ImGui::Button(this->label_.c_str(), size)) {
            triggerOnClick();
        }
    }
} // gui