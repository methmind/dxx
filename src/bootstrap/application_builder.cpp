//
// Created by sexey on 29.10.2025.
//

#include "application_builder.h"

#include "debug/debug_output.h"
#include "gui/gui_widget_regedit.h"
#include "gui/widget/gui_widget_root.h"
#include "home/home_directory.h"
#include "hook/hook_dispatcher.h"
#include "hook/hook_manager.h"
#include "input/bind_system.h"
#include "lua/lua_script_manager.h"
#include "renderer/renderer.h"
#include "sdk/dota_view_render.h"
#include "service_locator/service_container.h"
#include "service_locator/service_locator.h"

REGISTER_GLOBAL_SERVICE(hook::C_HookDispatcher);
REGISTER_GLOBAL_SERVICE(input::C_BindSystem);

namespace bootstrap
{
    std::shared_ptr<C_ServiceContainer> C_ApplicationBuilder::Build()
    {
        if (!C_ServiceLocator::getInstance<sdk::C_DotaViewRender>()->initialize()) {
            dbg("Unable to initialize sdk::C_DotaViewRender");
            return nullptr;
        }

        auto container = std::make_shared<C_ServiceContainer>();
        if (!container->add<hook::C_HookManager>()->initialize()) {
            dbg("Unable to initialize hooks!");
            return nullptr;
        }

        const auto widgetRegedit = container->add<gui::C_WidgetRegedit>();
        if (!container->add<lua::C_LuaScriptManager>(widgetRegedit)->initialize()) {
            dbg("Unable to initialize lua::C_LuaScriptEngine");
            return nullptr;
        }

        if (!InitializeGuiStuff(container)) {
            dbg("Unable to initialize GUI stuff!");
            return nullptr;
        }

        if (!container->add<render::C_Renderer>(widgetRegedit->find(gui::widget::ROOT_WIDGET_ID))->initialize()) {
            dbg("Unable to initialize renderer!");
            return nullptr;
        }

        return container;
    }
} // bootstrap