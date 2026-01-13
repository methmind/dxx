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

        C_IContainer::render();

        ImGui::EndMainMenuBar();
    }
} // gui
