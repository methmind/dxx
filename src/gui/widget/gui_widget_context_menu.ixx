//
// Created by sexey on 18.02.2026.
//
module;
#include <format>
#include <string>

#include "imgui.h"

export module gui.widget.context_menu;

import gui.widget.container;

namespace gui
{
    export class C_WidgetContextMenu : public C_ContainerBase
    {
    public:
        ~C_WidgetContextMenu() override = default;

        explicit C_WidgetContextMenu(const std::string_view& id, const std::string_view& label) :
            C_ContainerBase(id), label_(std::format("{}##{}", label, id)) {}

        void render() override
        {
            if (!getVisibleState()) {
                return;
            }

            if (ImGui::BeginMenu(this->label_.c_str())) {
                C_ContainerBase::render();
                ImGui::EndMenu();
            }
        }

    private:
        std::string label_;
    };
}