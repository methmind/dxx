//
// Created by sexey on 18.02.2026.
//
module;
#include <string_view>

#include "imgui.h"

export module gui.widget.windows_container;

import gui.widget.container;

namespace gui
{
    export class C_WidgetWindowsContainer final : public C_ContainerBase
    {
    public:
        explicit C_WidgetWindowsContainer(const std::string_view& id) : C_ContainerBase(id) {}

        void render() override
        {
            if (!getVisibleState()) {
                return;
            }

            if (getChildList().empty()) {
                ImGui::TextDisabled("N/A");
                return;
            }

            C_ContainerBase::render();
        }
    };
}