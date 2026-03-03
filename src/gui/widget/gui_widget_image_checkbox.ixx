//
// Created by sexey on 03.03.2026.
//
module;
#include <string_view>

#include "imgui.h"

export module gui.widget.image_checkbox;

import gui.widget.checkbox;
import renderer.d3d_texture;

namespace gui
{
    constexpr ImVec2 DEFAULT_TEXTURE_SIZE = { 35.0f, 35.0f };

    constexpr ImVec4 UNSELECTED_COLOR = { 1.0f, 0.0f, 0.0f, 1.0f };

    constexpr ImVec4 DEFAULT_TINT_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };

    export class C_WidgetImageCheckbox : public C_WidgetCheckbox
    {
    public:
        ~C_WidgetImageCheckbox() override = default;

        explicit C_WidgetImageCheckbox(const std::string_view& id) :
            C_WidgetCheckbox(id, ""), texture_(nullptr), textureSize_(DEFAULT_TEXTURE_SIZE) {}

        void setTextureSize(const ImVec2& size) { this->textureSize_ = size; }

        void setTexture(const render::d3d_texture_ptr_t& texture) { this->texture_ = texture; }

        void render() override
        {
            if (!getVisibleState())  {
                return;
            }

            if (!this->texture_) {
                return;
            }

            auto borderColor = UNSELECTED_COLOR;
            if (getState()) {
                borderColor = ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive);
            }

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_Button, borderColor);

            if (ImGui::ImageButton(getID().c_str(), this->texture_.get(), this->textureSize_,
                ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f),
                ImGui::GetStyleColorVec4(ImGuiCol_WindowBg), DEFAULT_TINT_COLOR)) {
                setState(!getState());
            }

            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
        }

    private:
        render::d3d_texture_ptr_t texture_;
        ImVec2 textureSize_;
    };
}