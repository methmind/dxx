//
// Created by sexey on 22.10.2025.
//

#ifndef MENU_MAIN_FORM_H
#define MENU_MAIN_FORM_H

#include "menu_main_form_designer.h"
#include "gui/widget/gui_widget_menu_item.h"
#include "input/bind_system.h"

namespace menu
{
    class C_MenuMainForm final : public C_MenuMainFormDesigner
    {
    private:
        std::shared_ptr<C_IMenuForm> settingsForm_;
        std::unique_ptr<input::C_KeyBindHandle> showBind_;

        void onSettingButtonClick(gui::C_IClickable* obj) override { this->settingsForm_->setVisible(true); }

        void menuShowBinding() { setVisible(!isVisible()); }

    public:

        bool initialize() override;

        std::shared_ptr<gui::widget::C_WidgetWindowsContainer>& getWindowsContainer()
        {
            return this->windowsContainer_;
        }

        explicit C_MenuMainForm(const std::shared_ptr<C_IMenuForm>& settingsForm) :
            C_IWidget(MAIN_FORM_ID), settingsForm_(settingsForm) {}

        ~C_MenuMainForm() override {}
    };
} // menu

#endif //MENU_MAIN_FORM_H