//
// Created by sexey on 18.02.2026.
//
module;
#include <string_view>

#include "imgui.h"

export module gui.widget.child_window;

import gui.widget.container;

namespace gui
{
    export class C_WidgetChildWindow : public C_ContainerBase
    {
    public:
        ~C_WidgetChildWindow() override = default;

        explicit C_WidgetChildWindow(const std::string_view& id) :
            C_ContainerBase(id), windowFlags_(ImGuiWindowFlags_None), childFlags_(ImGuiChildFlags_None) {}

        void setSize(const ImVec2& size) { this->size_ = size; }

        void setWindowFlags(const ImGuiWindowFlags flags) { this->windowFlags_ = flags; }

        void setChildFlags(const ImGuiChildFlags flags) { this->childFlags_ = flags; }

        void render() override
        {
            if (!getVisibleState()) {
                return;
            }

            if (ImGui::BeginChild(getID().c_str(), this->size_, this->childFlags_, this->windowFlags_)) {
                C_ContainerBase::render();
            }

            ImGui::EndChild();
        }

    private:
        ImVec2 size_;
        ImGuiWindowFlags windowFlags_;
        ImGuiChildFlags childFlags_;
    };
}