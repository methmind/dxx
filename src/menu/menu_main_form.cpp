//
// Created by sexey on 22.10.2025.
//

#include "menu_main_form.h"

#include <windows.h>
#include "gui/gui_widget_regedit.h"
#include "service_locator/service_locator.h"

namespace menu
{
    bool C_MenuMainForm::initialize()
    {
        this->showBind_ = std::move(C_ServiceLocator::getInstance<input::C_BindSystem>()->createBinding(
            VK_INSERT, [this] { menuShowBinding(); })
        );

        return C_MenuMainFormDesigner::initialize();
    }
} // menu