//
// Created by sexey on 01.03.2026.
//
module;
#include <format>
#include <string_view>
#include <memory>

export module as.binding.widget_window;

import as.binding.memory;

import gui.widget.window;
import gui.widget.menu_item;
import gui.widget.clickable;
import gui.widget.container;

namespace as
{
    export class C_ASBindingWidgetWindow : public gui::C_WidgetWindow
    {
    public:
        //@note Деструктор будет вызываться в очереди. Так что краша быть не должно.
        ~C_ASBindingWidgetWindow() override
        {
            const auto parent = this->shownButton_->getParent().lock();
            if (!parent) {
                return;
            }

            std::static_pointer_cast<C_ContainerBase>(parent)->removeChild(this->shownButton_);
        }

        explicit C_ASBindingWidgetWindow(
            const std::string_view& id,
            const std::string_view& title
        ) : C_WidgetWindow(id, title), shownButton_(std::make_shared<gui::C_WidgetMenuItem>(std::format("{}_show_button", id), title))
        {
            this->shownButton_->setCallback([this](gui::C_ClickableBase* obj) {
                this->setVisibleState(true);
            });
        }

        [[nodiscard]] std::shared_ptr<gui::C_WidgetMenuItem> getShownButton() const { return this->shownButton_; }

    private:
        std::shared_ptr<gui::C_WidgetMenuItem> shownButton_;
    };
}