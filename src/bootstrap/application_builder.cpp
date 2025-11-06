//
// Created by sexey on 29.10.2025.
//

#include "application_builder.h"

#include "debug/debug_output.h"
#include "gui/gui_widget_regedit.h"
#include "gui/widget/gui_widget_checkbox.h"
#include "home/home_directory.h"
#include "lua/lua_script_manager.h"
#include "service_locator/service_container.h"
#include "service_locator/service_locator.h"

REGISTER_GLOBAL_SERVICE(gui::C_WidgetRegedit);

namespace bootstrap
{
    std::shared_ptr<C_ServiceContainer> C_ApplicationBuilder::Build()
    {
        auto container = std::make_shared<C_ServiceContainer>();
        if (!container->add<home::C_HomeDirectory>()->initialize()) {
            dbg("Unable to initialize home::C_HomeDirectory");
            return nullptr;
        }

        const auto widgetRegedit = container->add<gui::C_WidgetRegedit>();
        if (!container->add<lua::C_LuaScriptManager>(widgetRegedit)->initialize()) {
            dbg("Unable to initialize lua::C_LuaScriptEngine");
            return nullptr;
        }

        return container;
    }
} // bootstrap