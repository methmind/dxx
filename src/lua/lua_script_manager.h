//
// Created by sexey on 26.10.2025.
//

#ifndef LUA_SCRIPT_MANAGER_H
#define LUA_SCRIPT_MANAGER_H

#include "lua_script_engine.h"
#include "lua_script_instance.h"
#include "binding/lua_container_interface.h"

namespace lua
{
    class C_LuaScriptManager : public binding::C_ILuaContainer, public std::enable_shared_from_this<C_LuaScriptManager>
    {
    private:
        C_LuaScriptEngine engine_;
        std::shared_ptr<gui::C_WidgetRegedit> widgetRegedit_;
        std::unordered_map<std::string_view, std::unique_ptr<C_LuaScriptInstance>> scripts_;

        bool bindWidget(const std::string_view& luaID, const gui::widget_ptr_t& widget) override;

    public:

        void disposeScript(const std::string_view& scriptPath);

        bool loadScript(const std::string_view& scriptPath);

        bool isScriptLoaded(const std::string_view& scriptPath) const { return this->scripts_.contains(scriptPath); }

        bool initialize();

        explicit C_LuaScriptManager(const std::shared_ptr<gui::C_WidgetRegedit>& widgetRegedit) :
            widgetRegedit_(widgetRegedit) {}

        ~C_LuaScriptManager() override = default;
    };
} // lua

#endif //LUA_SCRIPT_MANAGER_H