//
// Created by sexey on 26.11.2025.
//

#include "application_builder.h"
#include "lua/lua_script_manager.h"
#include "lua/binding/lua_binding_hook.h"
#include "lua/binding/lua_binding_imgui.h"
#include "lua/binding/lua_binding_menu.h"
#include "lua/binding/lua_binding_renderer.h"
#include "lua/binding/lua_binding_sdk_entities.h"
#include "lua/binding/lua_binding_sdk_math.h"
#include "lua/binding/lua_binding_sdk_schema.h"
#include "lua/binding/lua_binding_source_engine.h"
#include "sdk/custom/sdk_entity_list.h"

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

        luaEngine->addBinding(std::make_unique<lua::binding::C_LuaBindingSdkMath>());
        luaEngine->addBinding(std::make_unique<lua::binding::C_LuaBindingSdkSchema>());
        luaEngine->addBinding(std::make_unique<lua::binding::C_LuaBindingSourceEngine>());
        luaEngine->addBinding(std::make_unique<lua::binding::C_LuaBindingSdkEntities>(
            container->get<sdk::custom::C_EntityList>()
        ));

        luaEngine->addBinding(std::make_unique<lua::binding::C_LuaBindingImgui>());
        luaEngine->addBinding(std::make_unique<lua::binding::C_LuaBindingRenderer>(
            container->get<render::C_Renderer>()
        ));
        luaEngine->addBinding(std::make_unique<lua::binding::C_LuaBindingMenu>(
            container->get<gui::C_WidgetRegedit>(), luaManager
        ));

        return true;
    }
} // bootstrap