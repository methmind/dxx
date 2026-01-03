//
// Created by sexey on 28.10.2025.
//

#ifndef LUA_BINDING_MENU_H
#define LUA_BINDING_MENU_H

#include "lua_binding_interface.h"
#include "lua/lua_container_interface.h"
#include "gui/gui_widget_regedit.h"
#include "lua_guarded_state_interface.h"

namespace lua::binding
{
    constexpr auto MENU_NAMESPACE_NAME = "menu";

    class C_LuaBindingWidgetWrapper : public std::any
    {
    private:
        std::shared_ptr<gui::C_IWidget> instance_;
        std::shared_ptr<gui::C_WidgetRegedit> regedit_;

    public:

        explicit C_LuaBindingWidgetWrapper(const std::shared_ptr<gui::C_IWidget>& widget, const std::shared_ptr<gui::C_WidgetRegedit>& regedit) :
            instance_(widget), regedit_(regedit) {}

        ~C_LuaBindingWidgetWrapper() { this->regedit_->remove(this->instance_->getID()); }
    };

    class C_LuaBindingMenu final : public C_ILuaBinding
    {
    private:
        std::shared_ptr<gui::C_WidgetRegedit> widgetRegedit_;
        std::weak_ptr<C_ILuaContainer> luaContainer_;

        static void RegisterBasicInterfaces(sol::state& state);

        static void RegisterWindowWidgets(sol::state& state);

        static void RegisterClickableWidgets(sol::state& state);

        static void RegisterSliderWidgets(sol::state& state, const std::weak_ptr<C_ILuaGuardedState>& guardedState);

        static void RegisterSimpleWidgets(sol::state& state);

    public:

        bool apply(const std::weak_ptr<C_ILuaGuardedState>& guardedState) override;

        C_LuaBindingMenu(const std::shared_ptr<gui::C_WidgetRegedit>& widgetRegedit,
            const std::weak_ptr<C_ILuaContainer>& luaContainer)
            : widgetRegedit_(widgetRegedit), luaContainer_(luaContainer) {}

        ~C_LuaBindingMenu() override = default;
    };
} // lua

#endif //LUA_BINDING_MENU_H