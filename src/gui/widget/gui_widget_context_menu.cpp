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
            for (auto& childVector = getChildList(); const auto& child : childVector) {
                child->render();
            }

            ImGui::EndMenu();
        }
    }
} // gui