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
        const auto guiRootObject = guiRegedit->createWidget<gui::widget::C_WidgetRoot>();

        auto settingsForm = guiRegedit->createWidget<menu::C_MenuSettingsForm>(guiRegedit,
            services->get<lua::C_LuaScriptManager>()
        );

        const auto mainForm = guiRegedit->createWidget<menu::C_MenuMainForm>(settingsForm);
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