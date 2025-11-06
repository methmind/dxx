//
// Created by sexey on 28.10.2025.
//

#include "gui_widget_root.h"

namespace gui::widget
{
    void C_WidgetRoot::render()
    {
        for (auto& childVector = getChildList(); const auto& child : childVector) {
            child->render();
        }
    }
} // gui