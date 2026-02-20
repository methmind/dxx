//
// Created by sexey on 18.02.2026.
//
module;
#include <format>
#include <string_view>

#include "imgui.h"

export module gui.widget.window;

import gui.widget.container;

namespace gui
{
    export class C_WidgetWindow : public C_ContainerBase
    {
    public:
        ~C_WidgetWindow() override = default;

        explicit C_WidgetWindow(const std::string_view& id, const std::string_view& title) :
            C_ContainerBase(id), title_(std::format("{}##{}", title, id)),
            isRectInvalidated_(false) {}

        [[nodiscard]] const ImVec2& getPosition() const { return this->pos_; }

        [[nodiscard]] const ImVec2& getSize() const { return this->size_; }

        void setPosition(const ImVec2& pos)
        {
            this->pos_ = pos;
            this->isRectInvalidated_ = true;
        }

        void setSize(const ImVec2& size)
        {
            this->size_ = size;
            this->isRectInvalidated_ = true;
        }

        void render() override
        {
            if (!getVisibleState()) {
                return;
            }

            if (this->isRectInvalidated_) {
                ImGui::SetNextWindowPos(this->pos_, ImGuiCond_Always);
                ImGui::SetNextWindowSize(this->size_, ImGuiCond_Always);
                this->isRectInvalidated_ = false;
            }

            bool isStillActive = true;
            if (ImGui::Begin(this->title_.c_str(), &isStillActive, ImGuiWindowFlags_NoResize)) {
                const auto newPos = ImGui::GetWindowPos();
                const auto newSize = ImGui::GetWindowSize();

                if (newPos.x != this->pos_.x || newPos.y != this->pos_.y
                    || newSize.x != this->size_.x || newSize.y != this->size_.y) {
                    this->pos_ = newPos;
                    this->size_ = newSize;
                    this->isRectInvalidated_ = true;
                }

                C_ContainerBase::render();
            }

            if (!isStillActive) {
                setVisibleState(false);
            }

            ImGui::End();
        }

    private:
        std::string title_;

        ImVec2 pos_;
        ImVec2 size_;
        bool isRectInvalidated_;
    };
}
