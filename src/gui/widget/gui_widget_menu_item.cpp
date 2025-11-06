//
// Created by sexey on 22.10.2025.
//

#include "gui_widget_menu_item.h"

#include "imgui.h"

namespace gui::widget
{
    void C_WidgetMenuItem::render()
    {
        if (ImGui::MenuItem(this->label_.c_str())) {
            triggerOnClick();
        }
    }
} // gui