//
// Created by sexey on 17.02.2026.
//
module;
#include <string_view>

#include "imgui.h"

export module gui.widget.navbar;

import gui.widget.container;

namespace gui
{
    export class C_WidgetNavbar : public C_ContainerBase
    {
    public:
        explicit C_WidgetNavbar(const std::string_view& id) : C_ContainerBase(id) {}

        void render() override
        {
            if (!getVisibleState()) {
                return;
            }

            if (!ImGui::BeginMainMenuBar()) {
                return;
            }

            C_ContainerBase::render();
            ImGui::EndMainMenuBar();
        }
    };
}