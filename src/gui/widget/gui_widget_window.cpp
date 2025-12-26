//
// Created by sexey on 21.10.2025.
//

#include "gui_widget_window.h"

namespace gui::widget
{
    void C_WidgetWindow::render()
    {
        if (!isVisible()) {
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

            C_IContainer::render();
        }

        if (!isStillActive) {
            setVisibleState(false);
        }

        ImGui::End();
    }
} // gui