//
// Created by sexey on 29.10.2025.
//

#ifndef MENU_SETTINGS_FORM_DESIGNER_H
#define MENU_SETTINGS_FORM_DESIGNER_H

#include "menu_form_interface.h"
#include "gui/gui_widget_regedit.h"
#include "gui/widget/gui_widget_button.h"
#include "gui/widget/gui_widget_checkbox.h"
#include "gui/widget/gui_widget_child_window.h"
#include "gui/widget/gui_widget_lua_list.h"
#include "gui/widget/gui_widget_same_line.h"
#include "gui/widget/gui_widget_separator.h"
#include "gui/widget/gui_widget_slider_int.h"
#include "gui/widget/gui_widget_unformatted_text.h"
#include "gui/widget/gui_widget_window.h"

namespace menu
{
    constexpr auto SETTINGS_FORM_ID = "menu_settings_form";

    class C_MenuSettingsFormDesigner : public C_IMenuForm
    {
    protected:
        std::shared_ptr<gui::widget::C_WidgetWindow> settingsWindow_;
        std::shared_ptr<gui::widget::C_WidgetSeparator> luaDockTitle_;
        std::shared_ptr<gui::widget::C_WidgetChildWindow> luaDockPanel_;
        std::shared_ptr<gui::widget::C_WidgetLuaList> luaList_;
        std::shared_ptr<gui::widget::C_WidgetSameLine> dockComposer_;
        std::shared_ptr<gui::widget::C_WidgetSeparator> mainDockTitle_;
        std::shared_ptr<gui::widget::C_WidgetChildWindow> secondDockContainer_;
        std::shared_ptr<gui::widget::C_WidgetChildWindow> mainDockPanel_;
        std::shared_ptr<gui::widget::C_WidgetCheckbox> humanaizerCheckbox_;
        std::shared_ptr<gui::widget::C_WidgetCheckbox> silentMouseCheckbox_;
        std::shared_ptr<gui::widget::C_WidgetCheckbox> debugHumaizerCheckbox_;
        std::shared_ptr<gui::widget::C_WidgetCheckbox> outOfBoundsCheckbox_;
        std::shared_ptr<gui::widget::C_WidgetUnformattedText> humanaizerSmoothLabel_;
        std::shared_ptr<gui::widget::C_WidgetSliderInt> humanaizerSmooth_;
        std::shared_ptr<gui::widget::C_WidgetSeparator> configDockTitle_;
        std::shared_ptr<gui::widget::C_WidgetChildWindow> configDockPanel_;
        std::shared_ptr<gui::widget::C_WidgetButton> luaListUpdateButton_;
        std::shared_ptr<gui::widget::C_WidgetButton> configLoadButton_;
        std::shared_ptr<gui::widget::C_WidgetButton> configSaveButton_;

        virtual void onLuaRefreshListButtonClick(gui::C_IClickable* obj) = 0;

        virtual void onLuaButtonClick(gui::C_IClickable* obj) = 0;

        virtual void onConfigLoadButtonClick(gui::C_IClickable* obj) = 0;

        virtual void onConfigSaveButtonClick(gui::C_IClickable* obj) = 0;

    public:

        bool initialize() override;

        C_MenuSettingsFormDesigner() : C_IMenuForm(SETTINGS_FORM_ID) {}
    };
} // menu

#endif //MENU_SETTINGS_FORM_DESIGNER_H