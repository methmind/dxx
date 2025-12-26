//
// Created by sexey on 29.10.2025.
//

#ifndef MENU_MAIN_FORM_DESIGNER_H
#define MENU_MAIN_FORM_DESIGNER_H

#include "menu_form_interface.h"
#include "gui/widget/gui_widget_context_menu.h"
#include "gui/widget/gui_widget_menu_item.h"
#include "gui/widget/gui_widget_navbar.h"
#include "gui/widget/gui_widget_separator.h"
#include "gui/widget/gui_widget_windows_container.h"

namespace menu
{
    constexpr auto MAIN_FORM_ID = "menu_main_form";

    class C_MenuMainFormDesigner : public C_IMenuForm
    {
    protected:
        std::shared_ptr<gui::widget::C_WidgetNavbar> navbar_;
        std::shared_ptr<gui::widget::C_WidgetContextMenu> contextMenu_;
        std::shared_ptr<gui::widget::C_WidgetSeparator> scriptSeparator_;
        std::shared_ptr<gui::widget::C_WidgetWindowsContainer> windowsContainer_;
        std::shared_ptr<gui::widget::C_WidgetSeparator> miscSeparator_;
        std::shared_ptr<gui::widget::C_WidgetMenuItem> settingShownButton_;

        void serialize(nlohmann::json& document) override {}

        void deserialize(const nlohmann::json& document) override {}

        virtual void onSettingButtonClick(gui::C_IClickable* obj) = 0;

    public:

        bool initialize() override;

        C_MenuMainFormDesigner() : C_IMenuForm(MAIN_FORM_ID) {}
    };
} // menu

#endif //MENU_MAIN_FORM_DESIGNER_H