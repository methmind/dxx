//
// Created by sexey on 21.10.2025.
//

#include "gui_widget_navbar.h"

#include "imgui.h"

namespace gui::widget
{
    void C_WidgetNavbar::render()
    {
        if (!isVisible()) {
            return;
        }

        if (!ImGui::BeginMainMenuBar())
            return;

        for (auto& childVector = getChildList(); const auto& child : childVector) {
            child->render();
        }

        ImGui::EndMainMenuBar();
    }
} // gui
