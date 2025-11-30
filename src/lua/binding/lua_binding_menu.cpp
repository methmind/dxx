//
// Created by sexey on 28.10.2025.
//

#include "lua_binding_menu.h"

#include "debug/debug_output.h"
#include "service_locator/service_locator.h"
#include "gui/gui_clickable_interface.h"
#include "gui/gui_container_interface.h"
#include "gui/gui_slider_interface.h"
#include "gui/gui_widget_interface.h"
#include "gui/gui_widget_regedit.h"
#include "gui/widget/gui_widget_button.h"
#include "gui/widget/gui_widget_checkbox.h"
#include "gui/widget/gui_widget_child_window.h"
#include "gui/widget/gui_widget_menu_item.h"
#include "gui/widget/gui_widget_same_line.h"
#include "gui/widget/gui_widget_slider_float.h"
#include "gui/widget/gui_widget_slider_int.h"
#include "gui/widget/gui_widget_unformatted_text.h"
#include "gui/widget/gui_widget_window.h"
#include "gui/widget/gui_widget_windows_container.h"
#include "lua_guarded_state_interface.h"
#include "lua/lua_utils.h"
#include "menu/menu_main_form.h"
#include "menu/menu_main_form_designer.h"

namespace lua::binding
{
    void C_LuaBindingMenu::RegisterBasicInterfaces(sol::state& state)
    {
        auto luaIWidget = state.new_usertype<gui::C_IWidget>("C_IWidget", sol::no_constructor);
        luaIWidget.set_function("get_id", &gui::C_IWidget::getID);
        luaIWidget.set_function("get_type", &gui::C_IWidget::getType);
        luaIWidget.set_function("get_flags", &gui::C_IWidget::getFlags);
        luaIWidget.set_function("set_visible", &gui::C_IWidget::setVisible);
        luaIWidget.set_function("is_visible", &gui::C_IWidget::isVisible);

        auto luaIContainer = state.new_usertype<gui::C_IContainer>("C_IContainer", sol::no_constructor,
            sol::base_classes, sol::bases<gui::C_IWidget>()
        );
        luaIContainer.set_function("add_child", &gui::C_IContainer::addChild);

        auto luaIClickable = state.new_usertype<gui::C_IClickable>(
            "C_IClickable", sol::no_constructor,
            sol::base_classes, sol::bases<gui::C_IWidget>()
        );
        luaIClickable.set_function("get_state", &gui::C_IClickable::getState);
    }

    void C_LuaBindingMenu::RegisterWidgets(sol::state& state)
    {
        auto luaWindowWidget = state.new_usertype<gui::widget::C_WidgetWindow>(
            "C_WidgetWindow", sol::no_constructor,
            sol::base_classes, sol::bases<gui::C_IContainer, gui::C_IWidget>()
        );
        luaWindowWidget.set_function("get_position", &gui::widget::C_WidgetWindow::getPosition);
        luaWindowWidget.set_function("get_size", &gui::widget::C_WidgetWindow::getSize);
        luaWindowWidget.set_function("set_position", &gui::widget::C_WidgetWindow::setPosition);
        luaWindowWidget.set_function("set_size", &gui::widget::C_WidgetWindow::setSize);

        state.new_usertype<gui::widget::C_WidgetUnformattedText>(
            "C_WidgetUnformattedText", sol::no_constructor,
            sol::base_classes, sol::bases<gui::C_IWidget>()
        );

        state.new_usertype<gui::widget::C_WidgetSeparator>(
            "C_WidgetSeparator", sol::no_constructor,
            sol::base_classes, sol::bases<gui::C_IWidget>()
        );

        state.new_usertype<gui::widget::C_WidgetSameLine>(
            "C_WidgetSameLine", sol::no_constructor,
            sol::base_classes, sol::bases<gui::C_IWidget>()
        );

        auto luaButtonWidget = state.new_usertype<gui::widget::C_WidgetButton>(
            "C_WidgetButton", sol::no_constructor,
            sol::base_classes, sol::bases<gui::C_IClickable, gui::C_IWidget>()
        );
        luaButtonWidget.set_function("set_width_mode", &gui::widget::C_WidgetButton::setWidthMode);

        state.new_usertype<gui::widget::C_WidgetCheckbox>(
            "C_WidgetCheckbox", sol::no_constructor,
            sol::base_classes, sol::bases<gui::C_IClickable, gui::C_IWidget>()
        );

        auto luaChildWindowWidget = state.new_usertype<gui::widget::C_WidgetChildWindow>(
            "C_WidgetChildWindow", sol::no_constructor,
            sol::base_classes, sol::bases<gui::C_IContainer, gui::C_IWidget>()
        );
        luaChildWindowWidget.set_function("set_size", &gui::widget::C_WidgetChildWindow::setSize);

        auto luaSliderIntWidget = state.new_usertype<gui::widget::C_WidgetSliderInt>(
            "C_WidgetSliderInt", sol::no_constructor,
            sol::base_classes, sol::bases<gui::C_ISlider<int>>()
        );
        luaSliderIntWidget.set_function("set_value", &gui::widget::C_WidgetSliderInt::setValue);

        auto luaSliderFloatWidget = state.new_usertype<gui::widget::C_WidgetSliderFloat>(
            "C_WidgetSliderFloat", sol::no_constructor,
            sol::base_classes, sol::bases<gui::C_ISlider<float>>()
        );
        luaSliderFloatWidget.set_function("set_value", &gui::widget::C_WidgetSliderFloat::setValue);
    }

    template<typename widget_t, typename... args_t>
    gui::widget_ptr_t CreateWidgetHelper(sol::this_state state, const std::shared_ptr<gui::C_WidgetRegedit>& widgetRegedit,
        C_ILuaContainer* luaContainer, const gui::widget_ptr_t& parent,
        std::invocable<std::shared_ptr<widget_t>&> auto&& customizer, args_t&&... args
    )
    {
        if (!parent || !(parent->getFlags() & gui::widget_flags_e::CONTAINER)) {
            luaL_error(state.lua_state(), "Invalid parent widget!");
            __builtin_unreachable();
        }

        const auto luaPath = GetScriptPath(state.lua_state());
        if (luaPath.empty()) {
            luaL_error(state.lua_state(), "Unable to find lua path");
            __builtin_unreachable();
        }

        auto newWidget = widgetRegedit->createWidget<widget_t>(std::forward<args_t>(args)...);
        if (!newWidget) {
            luaL_error(state.lua_state(), "Unable to create new widget!");
            __builtin_unreachable();
        }

        std::invoke(std::forward<decltype(customizer)>(customizer), newWidget); // Call widget decorator

        std::dynamic_pointer_cast<gui::C_IContainer>(parent)->addChild(newWidget);
        const auto scriptInstance = luaContainer->getScriptInstance(luaPath);
        if (!scriptInstance) {
            luaL_error(state.lua_state(), "Unable to find lua script instance!");
            __builtin_unreachable();
        }

        scriptInstance->addDependency(std::make_shared<C_LuaBindingWidgetWrapper>(newWidget, widgetRegedit));
        return newWidget;
    }

    bool C_LuaBindingMenu::apply(const std::weak_ptr<C_ILuaGuardedState>& guardedState)
    {
        const auto tmp = guardedState.lock()->getLuaState();
        auto& luaState = *tmp;

        auto menuNamespace = luaState[MENU_NAMESPACE_NAME].get_or_create<sol::table>();
        if (!menuNamespace.valid()) {
            dbg("Unable to create menu namespace!");
            return false;
        }

        RegisterBasicInterfaces(luaState);
        RegisterWidgets(luaState);

        menuNamespace.set_function("get_widget", [this](const std::string_view& id){
            return this->widgetRegedit_->find(id);
        });

        menuNamespace.set_function("create_window",
            [this](sol::this_state state, const gui::widget_ptr_t& parent,
            const std::string_view& id, const std::string_view& label, const ImVec2 pos, const ImVec2 size, bool isVisible) {
                const auto mainForm = this->widgetRegedit_->find<menu::C_MenuMainForm>(menu::MAIN_FORM_ID);
                if (!mainForm) {
                    luaL_error(state.lua_state(), "Unable to find menu::C_MenuMainForm!");
                    __builtin_unreachable();
                }

                auto newWnd = CreateWidgetHelper<gui::widget::C_WidgetWindow>(
                state, this->widgetRegedit_, this->luaContainer_.lock().get(), parent,
                [&](const std::shared_ptr<gui::widget::C_WidgetWindow>& wnd) {
                    wnd->setPosition(pos);
                    wnd->setSize(size);
                }, id, label, isVisible);

                const auto showButton = std::dynamic_pointer_cast<gui::widget::C_WidgetMenuItem>(
                    CreateWidgetHelper<gui::widget::C_WidgetMenuItem>(
                    state, this->widgetRegedit_,this->luaContainer_.lock().get(),
                    mainForm->getWindowsContainer(),
                    [&](const std::shared_ptr<gui::widget::C_WidgetMenuItem>& item){},
                    std::format("{}_button", id.data()), label.data())
                );

                showButton->setCallback([newWnd](gui::C_IClickable*) {
                    newWnd->setVisible(true);
                });

                return newWnd;
            }
        );

        return true;
    }
} // lua