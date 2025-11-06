//
// Created by sexey on 01.11.2025.
//

#include "gui_widget_windows_container.h"

#include "imgui.h"

namespace gui::widget
{
    void C_WidgetWindowsContainer::render()
    {
        if (!isVisible()) {
            return;
        }

        auto& childVector = getChildList();
        if (childVector.empty()) {
            ImGui::TextDisabled("N/A");
            return;
        }

        for (const auto& child : childVector) {
            child->render();
        }
    }
} // gui