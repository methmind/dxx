//
// Created by sexey on 18.02.2026.
//
module;
#include <string_view>
#include <memory>

export module menu.settings.designer;

import menu;
import gui.widget.container;
import gui.widget.window;
import gui.widget.separator;
import gui.widget.child_window;
import gui.widget.same_line;
import gui.widget.checkbox;
import gui.widget.slider;
import gui.widget.text;
import gui.widget.button;
import gui.widget.script_list;

namespace menu
{
    export class C_MenuSettingsFormDesigner : public C_IForm
    {
    public:
        ~C_MenuSettingsFormDesigner() override = default;

        explicit C_MenuSettingsFormDesigner(const std::string_view& id) : C_IForm(id) {}

        void setVisibleState(const bool isVisible) override { this->settingsWindow_->setVisibleState(isVisible); }

        bool initialize() override
        {
            this->settingsWindow_ = std::make_shared<gui::C_WidgetWindow>("settings_form", "Settings");

            const auto asDockTitle = std::make_shared<gui::C_WidgetSeparator>("as_dock_title", "Scripts");
            const auto asDockPanel = std::make_shared<gui::C_WidgetChildWindow>("as_dock_panel");
            this->scriptList_ = std::make_shared<gui::C_WidgetScriptList>("as_script_list");

            const auto settingsDockTitle = std::make_shared<gui::C_WidgetSeparator>("settings_dock_title", "Settings");
            const auto settingsDockPanel = std::make_shared<gui::C_WidgetChildWindow>("settings_dock_panel");

            this->humanaizerCheckbox_ = std::make_shared<gui::C_WidgetCheckbox>("humanaizer_toggler", "Enable Humanaizer");
            this->silentMouseCheckbox_ = std::make_shared<gui::C_WidgetCheckbox>("silent_mouse_toggler",
            "Silent Mouse Move");
            this->outOfBoundsCheckbox_ = std::make_shared<gui::C_WidgetCheckbox>("out_of_bounds_toggler",
                "Disable OoB Orders");
            const auto humanaizerSmoothLabel = std::make_shared<gui::C_WidgetText>("humanaizer_smooth_label", "Humanaizer Smooth");
            this->humanaizerSmooth_ = std::make_shared<gui::C_SliderInt>("humanaizer_smooth_value");
            this->humanaizerSmooth_->setBounds(0, 1000);

            const auto controlDockTitle = std::make_shared<gui::C_WidgetSeparator>("control_dock_title", "Control");
            const auto controlDockPanel = std::make_shared<gui::C_WidgetChildWindow>("control_dock_panel");
            this->scriptListUpdateButton_ = std::make_shared<gui::C_WidgetButton>("as_list_update_button",
                "Update Script List"
            );
            this->scriptListUpdateButton_->setAutoWidthX(true);

            this->configLoadButton_ = std::make_shared<gui::C_WidgetButton>("config_load_button", "Load");
            this->configLoadButton_->setAutoWidthX(true);

            this->configSaveButton_ = std::make_shared<gui::C_WidgetButton>("config_save_button", "Save");
            this->configSaveButton_->setAutoWidthX(true);

            asDockPanel->addChild(asDockTitle);
            asDockPanel->setSize({250, 0});
            asDockPanel->addChild(this->scriptList_);

            settingsDockPanel->addChild(settingsDockTitle);
            settingsDockPanel->addChild(this->humanaizerCheckbox_);
            settingsDockPanel->addChild(this->silentMouseCheckbox_);
            settingsDockPanel->addChild(this->outOfBoundsCheckbox_);
            settingsDockPanel->addChild(humanaizerSmoothLabel);
            settingsDockPanel->addChild(this->humanaizerSmooth_);
            settingsDockPanel->addChild(controlDockPanel);

            controlDockPanel->addChild(controlDockTitle);
            controlDockPanel->addChild(this->scriptListUpdateButton_);
            controlDockPanel->addChild(this->configLoadButton_);
            controlDockPanel->addChild(this->configSaveButton_);

            this->settingsWindow_->setSize({500, 280});
            this->settingsWindow_->addChild(asDockPanel);
            this->settingsWindow_->addChild(std::make_shared<gui::C_WidgetSameLine>("dock_composer"));
            this->settingsWindow_->addChild(settingsDockPanel);

            addChild(this->settingsWindow_);
            return true;
        }

    protected:
        std::shared_ptr<gui::C_WidgetWindow> settingsWindow_;
        std::shared_ptr<gui::C_WidgetScriptList> scriptList_;

        std::shared_ptr<gui::C_WidgetCheckbox> humanaizerCheckbox_;
        std::shared_ptr<gui::C_WidgetCheckbox> silentMouseCheckbox_;
        std::shared_ptr<gui::C_WidgetCheckbox> outOfBoundsCheckbox_;
        std::shared_ptr<gui::C_SliderInt> humanaizerSmooth_;

        std::shared_ptr<gui::C_WidgetButton> scriptListUpdateButton_;
        std::shared_ptr<gui::C_WidgetButton> configLoadButton_;
        std::shared_ptr<gui::C_WidgetButton> configSaveButton_;
    };
}