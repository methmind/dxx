//
// Created by sexey on 26.11.2025.
//

#include "application_builder.h"
#include "lua/lua_script_manager.h"
#include "lua/binding/lua_binding_hook.h"
#include "lua/binding/lua_binding_menu.h"

namespace bootstrap
{
    bool C_ApplicationBuilder::InitializeLuaStuff(const std::shared_ptr<C_ServiceContainer>& container)
    {
        const auto luaManager = container->add<lua::C_LuaScriptManager>();
        if (!luaManager->initialize()) {
            dbg("Unable to initialize lua::C_LuaScriptEngine");
            return false;
        }

        const auto luaEngine = luaManager->getEngine();
        luaEngine->addBinding(std::make_unique<lua::binding::C_LuaBindingHook>());
        luaEngine->addBinding(std::make_unique<lua::binding::C_LuaBindingMenu>(
            container->get<gui::C_WidgetRegedit>(), luaManager
        ));

        return true;
    }
} // bootstrap