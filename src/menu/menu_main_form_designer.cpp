//
// Created by sexey on 29.10.2025.
//

#include "menu_main_form_designer.h"

namespace menu
{
    bool C_MenuMainFormDesigner::initialize()
    {
        this->navbar_ = std::make_shared<gui::widget::C_WidgetNavbar>("navbar");
        this->contextMenu_ = std::make_shared<gui::widget::C_WidgetContextMenu>("navbar_main_menu", "Dota++");
        this->scriptSeparator_ = std::make_shared<gui::widget::C_WidgetSeparator>("script_separator", "Scripts");
        this->windowsContainer_ = std::make_shared<gui::widget::C_WidgetWindowsContainer>("navbar_windows_container");
        this->miscSeparator_ = std::make_shared<gui::widget::C_WidgetSeparator>("settings_separator", "Settings");
        this->settingShownButton_ = std::make_shared<gui::widget::C_WidgetMenuItem>("main_menu_settings", "Settings");
        this->settingShownButton_->setCallback([this](gui::C_IClickable* obj){ onSettingButtonClick(obj); });

        this->contextMenu_->addChild(this->scriptSeparator_);
        this->contextMenu_->addChild(this->windowsContainer_);
        this->contextMenu_->addChild(this->miscSeparator_);
        this->contextMenu_->addChild(this->settingShownButton_);
        this->navbar_->addChild(this->contextMenu_);

        addChild(this->navbar_);

        return true;
    }
} // menu