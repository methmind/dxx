//
// Created by sexey on 26.10.2025.
//

#ifndef LUA_SCRIPT_MANAGER_H
#define LUA_SCRIPT_MANAGER_H

#include "lua_script_engine.h"
#include "lua_script_instance.h"
#include "lua_container_interface.h"
#include "hash/xxhash_wrapper.h"

namespace lua
{
    class C_LuaScriptManager : public C_ILuaContainer, public std::enable_shared_from_this<C_LuaScriptManager>
    {
    private:
        std::shared_ptr<C_LuaScriptEngine> engine_;
        std::shared_ptr<gui::C_WidgetRegedit> widgetRegedit_;
        std::unordered_map<std::string, std::unique_ptr<C_LuaScriptInstance>, xx_hashier_s> scripts_;

        /*
         * Предполагаем, что lua state уже залочен на момент вызова этой функции.
         * Ибо это вызывается уже из контекста lua интерпретатора.
         * Не хочу использовать std::recursive_mutex...
         */
        bool bindGuiWidget(const std::string_view& luaID, const gui::widget_ptr_t& widget) override;

    public:

        std::shared_ptr<C_LuaScriptEngine> getEngine() { return this->engine_; }

        void disposeScript(const std::string_view& scriptPath);

        bool loadScript(const std::string_view& scriptPath);

        bool isScriptLoaded(const std::string_view& scriptPath) const { return this->scripts_.contains(scriptPath.data()); }

        bool initialize() const;

        explicit C_LuaScriptManager(const std::shared_ptr<gui::C_WidgetRegedit>& widgetRegedit) :
            engine_(std::make_shared<C_LuaScriptEngine>()),
            widgetRegedit_(widgetRegedit) {}

        ~C_LuaScriptManager() override = default;
    };
} // lua

#endif //LUA_SCRIPT_MANAGER_H