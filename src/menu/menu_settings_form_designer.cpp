//
// Created by sexey on 29.10.2025.
//

#include "menu_settings_form_designer.h"

#include "gui/widget/gui_widget_slider_float.h"

namespace menu
{
    bool C_MenuSettingsFormDesigner::initialize()
    {
        this->settingsWindow_ = std::make_shared<gui::widget::C_WidgetWindow>("settings_window", "Settings", false);

        this->luaDockTitle_ = std::make_shared<gui::widget::C_WidgetSeparator>("lua_dock_separator", "Lua");
        this->luaDockPanel_ = std::make_shared<gui::widget::C_WidgetChildWindow>("lua_dock_panel",
            ImGuiWindowFlags_None, ImGuiChildFlags_None);
        this->luaList_ = std::make_shared<gui::widget::C_WidgetLuaList>("lua_list_view");
        this->luaList_->setCallback([this](gui::C_IClickable* obj){ onLuaButtonClick(obj); });

        this->dockComposer_ = std::make_shared<gui::widget::C_WidgetSameLine>("");
        this->mainDockTitle_ = std::make_shared<gui::widget::C_WidgetSeparator>("main_dock_separator", "Main");

        this->secondDockContainer_ = std::make_shared<gui::widget::C_WidgetChildWindow>("second_dock_container",
            ImGuiWindowFlags_None, ImGuiChildFlags_None);
        this->mainDockPanel_ = std::make_shared<gui::widget::C_WidgetChildWindow>("main_dock_panel",
            ImGuiWindowFlags_None, ImGuiChildFlags_None);

        this->humanaizerCheckbox_ = std::make_shared<gui::widget::C_WidgetCheckbox>("humanaizer_toggler",
            "Enable Humanaizer");
        this->silentMouseCheckbox_ = std::make_shared<gui::widget::C_WidgetCheckbox>("silent_mouse_toggler",
            "Silent Mouse Move");
        this->debugHumaizerCheckbox_ = std::make_shared<gui::widget::C_WidgetCheckbox>("debug_humanaizer_toggler",
            "Debug Humanaizer");
        this->outOfBoundsCheckbox_ = std::make_shared<gui::widget::C_WidgetCheckbox>("out_of_bounds_toggler",
            "Disable OoB Orders");

        this->humanaizerSmoothLabel_ = std::make_shared<gui::widget::C_WidgetUnformattedText>("humanaizer_smoth_label", "Humanaizer Smooth");
        this->humanaizerSmooth_ = std::make_shared<gui::widget::C_WidgetSliderInt>("humanaizer_smooth");
        this->humanaizerSmooth_->setBounds(0, 1000);

        this->configDockTitle_ = std::make_shared<gui::widget::C_WidgetSeparator>("config_dock_separator", "Config");
        this->configDockPanel_ = std::make_shared<gui::widget::C_WidgetChildWindow>("config_dock_panel",
            ImGuiWindowFlags_None, ImGuiChildFlags_None);

        this->luaListUpdateButton_ = std::make_shared<gui::widget::C_WidgetButton>("lua_list_update_button",
            "Update Lua List"
        );
        this->luaListUpdateButton_->setCallback([this](gui::C_IClickable* obj){ onLuaRefreshListButtonClick(obj); });
        this->luaListUpdateButton_->setWidthMode(true);

        this->configLoadButton_ = std::make_shared<gui::widget::C_WidgetButton>("config_load_button", "Load");
        this->configLoadButton_->setCallback([this](gui::C_IClickable* obj){ onConfigLoadButtonClick(obj); });
        this->configLoadButton_->setWidthMode(true);

        this->configSaveButton_ = std::make_shared<gui::widget::C_WidgetButton>("config_save_button", "Save");
        this->configSaveButton_->setCallback([this](gui::C_IClickable* obj){ onConfigSaveButtonClick(obj); });
        this->configSaveButton_->setWidthMode(true);

        this->luaDockPanel_->setSize({250, 0});
        this->luaDockPanel_->addChild(this->luaDockTitle_);
        this->luaDockPanel_->addChild(this->luaList_);

        this->mainDockPanel_->setSize({0, 145});
        this->mainDockPanel_->addChild(this->humanaizerCheckbox_);
        this->mainDockPanel_->addChild(this->outOfBoundsCheckbox_);
        this->mainDockPanel_->addChild(this->silentMouseCheckbox_);
        this->mainDockPanel_->addChild(this->debugHumaizerCheckbox_);
        this->mainDockPanel_->addChild(this->humanaizerSmoothLabel_);
        this->mainDockPanel_->addChild(this->humanaizerSmooth_);

        this->configDockPanel_->setSize({0, 0});
        this->configDockPanel_->addChild(this->configDockTitle_);
        this->configDockPanel_->addChild(this->luaListUpdateButton_);
        this->configDockPanel_->addChild(this->configLoadButton_);
        this->configDockPanel_->addChild(this->configSaveButton_);

        this->secondDockContainer_->setSize({0, 0});
        this->secondDockContainer_->addChild(this->mainDockTitle_);
        this->secondDockContainer_->addChild(this->mainDockPanel_);
        this->secondDockContainer_->addChild(this->configDockPanel_);

        this->settingsWindow_->setSize({500, 310});
        this->settingsWindow_->addChild(this->luaDockPanel_);
        this->settingsWindow_->addChild(this->dockComposer_);
        this->settingsWindow_->addChild(this->secondDockContainer_);

        addChild(this->settingsWindow_);
        return true;
    }
} // menu