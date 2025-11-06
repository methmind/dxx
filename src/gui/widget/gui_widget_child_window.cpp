//
// Created by sexey on 23.10.2025.
//

#include "gui_widget_child_window.h"

#include "imgui.h"

namespace gui::widget
{
    void C_WidgetChildWindow::render()
    {
        if (!isVisible()) {
            return;
        }

        if (ImGui::BeginChild(getID().c_str(), this->size_, this->childFlags_, this->flags_)) {
            for (auto& childVector = getChildList(); const auto& child : childVector) {
                child->render();
            }
        }

        ImGui::EndChild();
    }
} // gui