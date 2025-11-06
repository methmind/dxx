//
// Created by sexey on 25.10.2025.
//

#include "gui_widget_checkbox.h"

#include "imgui.h"

namespace gui::widget
{
    void C_WidgetCheckbox::setState(const int32_t state)
    {
        this->checked_ = state;
        triggerOnClick(); //todo probably bad idea...
    }

    void C_WidgetCheckbox::render()
    {
        if (ImGui::Checkbox(this->label_.c_str(), &this->checked_)) {
            triggerOnClick();
        }
    }

    void C_WidgetCheckbox::deserialize(const nlohmann::json& document)
    {
        setState(document.at("state").get<bool>());
    }

    void C_WidgetCheckbox::serialize(nlohmann::json& document)
    {
        document["state"] = this->checked_;
    }
} // gui