//
// Created by sexey on 29.10.2025.
//

#include "application_builder.h"

#include "debug/debug_output.h"
#include "gui/gui_widget_regedit.h"
#include "gui/widget/gui_widget_root.h"
#include "input/bind_system.h"
#include "renderer/renderer.h"
#include "service_locator/service_container.h"
#include "service_locator/service_locator.h"

REGISTER_GLOBAL_SERVICE(input::C_BindSystem);

namespace bootstrap
{
    std::shared_ptr<C_ServiceContainer> C_ApplicationBuilder::Build()
    {
        auto container = std::make_shared<C_ServiceContainer>();
        if (!InitializeSdkStuff(container)) {
            dbg("Unable to initialize SDK stuff!");
            return nullptr;
        }

        if (!InitializeHookStuff()) {
            dbg("Unable to initialize hooks!");
            return nullptr;
        }

        const auto widgetRegedit = container->add<gui::C_WidgetRegedit>();
        const auto renderer = container->add<render::C_Renderer>();

        if (!InitializeLuaStuff(container)) {
            dbg("Unable to initialize Lua stuff!");
            return nullptr;
        }

        if (!InitializeGuiStuff(container)) {
            dbg("Unable to initialize GUI stuff!");
            return nullptr;
        }

        if (auto rootWidget = widgetRegedit->find(gui::widget::ROOT_WIDGET_ID);
            !renderer->initialize([rootWidget]{ rootWidget->render(); })) {
            dbg("Unable to initialize renderer!");
            return nullptr;
        }

        return container;
    }
} // bootstrap