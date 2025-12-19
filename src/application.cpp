//
// Created by sexey on 06.11.2025.
//

#include "application.h"

#include "bootstrap/application_builder.h"
#include "debug/debug_output.h"
#include "feature/feature_virtual_camera_manager.h"
#include "gui/gui_widget_regedit.h"
#include "gui/widget/gui_widget_root.h"
#include "hook/hook_dispatcher.h"
#include "hook/hook_manager.h"
#include "input/bind_system.h"
#include "menu/menu_settings_form.h"
#include "sdk/interface/sdk_interface_scanner.h"
#include "service_locator/service_locator.h"

namespace app
{
    bool C_Application::entry()
    {
        if (this->disposeEvent_ = CreateEventW(nullptr, FALSE, FALSE, nullptr); !this->disposeEvent_) {
            dbg("CreateEvent got:err = %d", GetLastError());
            return false;
        }

        const auto services = bootstrap::C_ApplicationBuilder::Build();
        if (!services) {
            dbg("Unable to initialize application!");
            return false;
        }

        if (!services->add<feature::C_FeatureVirtualCameraManager>(services->get<sdk::custom::C_MatricesSystem>(), services->get<render::C_Renderer>())->initialize()) {
            return false;
        }

        auto endBind = C_ServiceLocator::getInstance<input::C_BindSystem>()->createBinding(
            VK_END, [this] {
                SetEvent(this->disposeEvent_);
            }
        );

        if (!hook::C_HookManager::Enable()) {
            dbg("Unable to enable hooks!");
            return false;
        }

        if (const auto err = WaitForSingleObject(this->disposeEvent_, INFINITE); err != ERROR_SUCCESS) {
            dbg("WaitForSingleObject got:err = %d", err);
        }

        dbg("Unloading module from process...");
        hook::C_HookManager::Disable();
        C_ServiceLocator::getInstance<hook::C_HookDispatcher>()->clear(); // Освобождаем всех слушателей с хуков.

        return true;
    }
} // core