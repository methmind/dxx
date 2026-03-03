//
// Created by sexey on 03.03.2026.
//
module;
#include <format>
#include <string_view>

#include "imgui.h"

export module gui.widget.header;

import gui.widget.container;

namespace gui
{
    export class C_WidgetHeader : public C_ContainerBase
    {
    public:
        explicit C_WidgetHeader(const std::string_view& id, const std::string_view& label) :
            C_ContainerBase(std::format("{}##{}", label, id)) {}

        void render() override
        {
            if (!getVisibleState()) {
                return;
            }

            if (ImGui::CollapsingHeader(getID().c_str())) {
                C_ContainerBase::render();
            }
        }
    };
}