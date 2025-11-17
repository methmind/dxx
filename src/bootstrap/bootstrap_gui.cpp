//
// Created by sexey on 07.11.2025.
//

#include "application_builder.h"

#include "gui/gui_widget_regedit.h"
#include "gui/widget/gui_widget_root.h"
#include "menu/menu_main_form.h"
#include "menu/menu_settings_form.h"

namespace bootstrap
{
    bool C_ApplicationBuilder::InitializeGuiStuff(const std::shared_ptr<C_ServiceContainer>& services)
    {
        const auto guiRegedit = services->get<gui::C_WidgetRegedit>();
        const auto guiRootObject = std::dynamic_pointer_cast<gui::widget::C_WidgetRoot>(
            guiRegedit->createWidget<gui::widget::C_WidgetRoot>()
        );

        auto settingsForm = std::dynamic_pointer_cast<menu::C_MenuSettingsForm>(
            guiRegedit->createWidget<menu::C_MenuSettingsForm>(guiRegedit, services->get<lua::C_LuaScriptManager>())
        );

        const auto mainForm = std::dynamic_pointer_cast<menu::C_MenuMainForm>(
            guiRegedit->createWidget<menu::C_MenuMainForm>(settingsForm)
        );

        if (!mainForm->initialize()) {
            dbg("Unable to initialize main form!");
            return false;
        }

        if (!settingsForm->initialize()) {
            dbg("Unable to initialize main form!");
            return false;
        }

        mainForm->addChild(settingsForm);
        guiRootObject->addChild(mainForm);

        return true;
    }
} // bootstrap