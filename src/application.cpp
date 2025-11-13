//
// Created by sexey on 06.11.2025.
//

#include "application.h"

#include "bootstrap/application_builder.h"
#include "debug/debug_output.h"
#include "gui/gui_widget_regedit.h"
#include "gui/widget/gui_widget_root.h"
#include "hook/hook_manager.h"
#include "menu/menu_settings_form.h"
#include "sdk/interface/sdk_interface_scanner.h"

namespace app
{
    bool C_Application::entry(HMODULE hModule)
    {
        MessageBoxA(nullptr, "Press F to pay respect", nullptr, 0);

        if (this->disposeEvent_ = CreateEventW(nullptr, FALSE, FALSE, nullptr); !this->disposeEvent_) {
            dbg("CreateEvent got:err = %d", GetLastError());
            return false;
        }

        const auto services = bootstrap::C_ApplicationBuilder::Build();
        if (!services) {
            dbg("Unable to initialize application!");
            return false;
        }

        if (!services->get<hook::C_HookManager>()->enable()) {
            dbg("Unable to enable hooks!");
            return false;
        }

        return WaitForSingleObject(this->disposeEvent_, INFINITE) == ERROR_SUCCESS;
    }

    void C_Application::dispose() const
    {
        if (!this->disposeEvent_) {
            return;
        }

        SetEvent(this->disposeEvent_);
    }
} // core