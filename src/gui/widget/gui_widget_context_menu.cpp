//
// Created by sexey on 22.10.2025.
//

#include "gui_widget_context_menu.h"

#include "imgui.h"

namespace gui::widget
{
    void C_WidgetContextMenu::render()
    {
        if (!isVisible()) {
            return;
        }

        if (ImGui::BeginMenu(this->label_.c_str())) {
            C_IContainer::render();
            ImGui::EndMenu();
        }
    }
} // gui