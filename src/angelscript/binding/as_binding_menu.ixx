//
// Created by sexey on 28.02.2026.
//
module;
#include <format>
#include <memory>

#include "imgui.h"
#include "asbind20/asbind.hpp"

export module as.binding.menu;

import as.binding;
import as.binding.memory;
import as.engine_interface;
import as.binding.widget_window;

import gui.widget.base;
import gui.widget.container;
import gui.widget.window;
import gui.widget.separator;
import gui.widget.checkbox;
import gui.widget.slider;
import gui.widget.text;
import gui.widget.same_line;
import gui.widget.color_picker;

import gui.regedit;

import menu.navbar;
import worker_queue;

namespace as
{
    constexpr auto AS_MENU_NAMESPACE_NAME = "menu";

    using as_widget_base_t       = C_SharedPtr<gui::C_WidgetBase>;
    using as_widget_window_t     = C_SharedPtr<C_ASBindingWidgetWindow>;
    using as_widget_separator_t  = C_SharedPtr<gui::C_WidgetSeparator>;
    using as_widget_checkbox_t   = C_SharedPtr<gui::C_WidgetCheckbox>;
    using as_widget_slider_t     = C_SharedPtr<gui::C_SliderInt>;
    using as_widget_text_t       = C_SharedPtr<gui::C_WidgetText>;
    using as_widget_same_line_t  = C_SharedPtr<gui::C_WidgetSameLine>;
    using as_widget_clr_picker_t = C_SharedPtr<gui::C_WidgetColorPicker>;

    template<typename wrapper_t>
    auto registerRefType(asIScriptEngine* engine, const char* name)
    {
        return asbind20::ref_class<wrapper_t>(engine, name)
            .addref(&wrapper_t::addRef)
            .release(&wrapper_t::release);
    }

    // Регистрирует implicit upcast: Derived@ -> Base@
    template<typename T_DerivedWrapper, typename T_BaseWrapper, typename T_BaseImpl>
    void registerUpcast(asbind20::ref_class<T_DerivedWrapper>& binder, const std::string& baseTypeName)
    {
        binder.method(
            (baseTypeName + "@ opImplCast()").c_str(),
            [](T_DerivedWrapper* self) -> T_BaseWrapper* {
                return T_DerivedWrapper::template implicitCast<T_BaseImpl>(self);
            }
        );
    }

    // Регистрирует explicit downcast: Base@ -> Derived@
    template<typename T_BaseWrapper, typename T_DerivedWrapper, typename T_DerivedImpl>
    void registerDowncast(asbind20::ref_class<T_BaseWrapper>& binder, const std::string& derivedTypeName)
    {
        binder.method(
            (derivedTypeName + "@ opCast()").c_str(),
            [](T_BaseWrapper* self) -> T_DerivedWrapper* {
                return T_BaseWrapper::template implicitCast<T_DerivedImpl>(self);
            }
        );
    }

    export class C_ASBindingMenu : public C_IASBinding
    {
    public:
        ~C_ASBindingMenu() override = default;

        explicit C_ASBindingMenu(
            const std::shared_ptr<gui::C_WidgetRegedit>& regedit,
            const std::shared_ptr<C_WorkerQueue>& queue
        ) : regedit_(regedit), queue_(queue) {}

        [[nodiscard]] bool apply(const std::weak_ptr<C_IASEngine>& engineWeak) override
        {
            const auto enginePtr = engineWeak.lock();
            if (!enginePtr) {
                return false;
            }

            const auto engine = enginePtr->getEngine();
            engine->SetDefaultNamespace(AS_MENU_NAMESPACE_NAME);

            registerWidgetTypes(engine);
            registerFactoryFunctions(engine);

            engine->SetDefaultNamespace("");
            return true;
        }

    private:
        static void registerWidgetTypes(asIScriptEngine* engine)
        {
            registerRefType<as_widget_base_t>(engine, "C_WidgetBase");

            auto windowBinder = registerRefType<as_widget_window_t>(engine, "C_WidgetWindow")
                .method("imgui::ImVec2 getPosition()",
                    [](const as_widget_window_t& self) { return self->getPosition(); })
                .method("void setPosition(imgui::ImVec2)",
                    [](const as_widget_window_t& self, ImVec2 pos) { self->setPosition(pos); })
                .method("imgui::ImVec2 getSize()",
                    [](const as_widget_window_t& self) { return self->getSize(); })
                .method("void setSize(imgui::ImVec2)",
                    [](const as_widget_window_t& self, ImVec2 size) { self->setSize(size); });
            registerUpcast<as_widget_window_t, as_widget_base_t, gui::C_WidgetBase>(
                windowBinder, "C_WidgetBase"
            );

            registerRefType<as_widget_separator_t>(engine, "C_WidgetSeparator");

            registerRefType<as_widget_checkbox_t>(engine, "C_WidgetCheckbox")
                .method("bool getState()",
                    [](const as_widget_checkbox_t& self) { return self->getState(); })
                .method("void setState(bool)",
                    [](const as_widget_checkbox_t& self, bool state) { self->setState(state); });

            registerRefType<as_widget_slider_t>(engine, "C_WidgetSlider")
                .method("int getValue()",
                    [](const as_widget_slider_t& self) { return self->getValue(); })
                .method("void setValue(int)",
                    [](const as_widget_slider_t& self, int32_t value) { self->setValue(value); });

            registerRefType<as_widget_text_t>(engine, "C_WidgetText");
            registerRefType<as_widget_same_line_t>(engine, "C_WidgetSameLine");
            registerRefType<as_widget_clr_picker_t>(engine, "C_WidgetColorPicker")
                .method("imgui::ImVec4 getColor()",
                    [](const as_widget_clr_picker_t& self) { return self->getColor(); })
                .method("void setColor(imgui::ImVec4)",
                    [](const as_widget_clr_picker_t& self, ImVec4 color) { self->setColor(color); });
        }

        void registerFactoryFunctions(asIScriptEngine* engine)
        {
            asbind20::global(engine)
                .function("menu::C_WidgetWindow@ createWindow(const string& in, const string& in)",
                    &C_ASBindingMenu::createWindow, asbind20::auxiliary(this))
                .function("menu::C_WidgetSeparator@ createSeparator(const C_WidgetBase& in, const string& in, const string& in)",
                    &C_ASBindingMenu::createSeparator, asbind20::auxiliary(this))
                .function("menu::C_WidgetCheckbox@ createCheckbox(const C_WidgetBase& in, const string& in, const string& in)",
                    &C_ASBindingMenu::createCheckbox, asbind20::auxiliary(this))
                .function("menu::C_WidgetSlider@ createSlider(const C_WidgetBase& in, const string& in, int, int)",
                    &C_ASBindingMenu::createSlider, asbind20::auxiliary(this))
                .function("menu::C_WidgetText@ createText(const C_WidgetBase& in, const string& in)",
                    &C_ASBindingMenu::createText, asbind20::auxiliary(this))
                .function("menu::C_WidgetSameLine@ createSameLine(const C_WidgetBase& in, const string& in)",
                    &C_ASBindingMenu::createSameLine, asbind20::auxiliary(this))
                .function("menu::C_WidgetColorPicker@ createColorPicker(const C_WidgetBase& in, const string& in)",
                    &C_ASBindingMenu::createColorPicker, asbind20::auxiliary(this));
        }

        [[nodiscard]] static std::shared_ptr<gui::C_ContainerBase> resolveParent(const as_widget_base_t& ptr)
        {
            if (!ptr) {
                return nullptr;
            }

            return std::dynamic_pointer_cast<gui::C_ContainerBase>(ptr.get_shared());
        }

        template<typename T>
        [[nodiscard]] auto makeDisposeCallback() const
        {
            return [regedit = this->regedit_, queue = this->queue_](C_SharedPtr<T>* ptr) {
                if (!ptr || !ptr->get()) {
                    return;
                }

                const auto widgetShared = ptr->get_shared();
                queue->enqueue([widgetShared] {
                    const auto parent = widgetShared->getParent().lock();
                    if (!parent) {
                        return;
                    }

                    if (auto container = std::static_pointer_cast<gui::C_ContainerBase>(parent)) {
                        container->removeChild(widgetShared);
                    }
                });

                regedit->remove(widgetShared->getID());
            };
        }

        template<typename T, typename... args_t>
        [[nodiscard]] std::unique_ptr<C_SharedPtr<T>> createWidget(
            const std::shared_ptr<gui::C_ContainerBase>& parent, args_t&&... args) const
        {
            if (!parent) {
                asbind20::set_script_exception("Parent must be a container!");
                return nullptr;
            }

            auto widget = this->regedit_->createWidget<T>(std::forward<args_t>(args)...);
            if (!widget) {
                asbind20::set_script_exception("Failed to create widget!");
                return nullptr;
            }

            this->queue_->enqueue([parent, widget] {
                parent->addChild(widget);
            });

            return std::make_unique<C_SharedPtr<T>>(widget, makeDisposeCallback<T>());
        }

        // Общий паттерн для создания дочернего виджета: resolveParent → createWidget → release.
        template<typename T, typename... args_t>
        [[nodiscard]] C_SharedPtr<T>* createChildWidget(
            const as_widget_base_t& parentRef, args_t&&... args) const
        {
            const auto parent = resolveParent(parentRef);
            if (!parent) {
                asbind20::set_script_exception("Parent must be a container!");
                return nullptr;
            }

            auto result = createWidget<T>(parent, std::forward<args_t>(args)...);
            if (!result) {
                asbind20::set_script_exception("Failed to create widget!");
                return nullptr;
            }

            return result.release();
        }

        [[nodiscard]] as_widget_window_t* createWindow(const std::string& id, const std::string& title) const
        {
            const auto navbar = this->regedit_->find<menu::C_MenuNavbarForm>(menu::MENU_NAVBAR_ID);
            if (!navbar) {
                asbind20::set_script_exception("Navbar not found in regedit!");
                return nullptr;
            }

            auto window = createWidget<C_ASBindingWidgetWindow>(navbar, id, title);
            if (!window) {
                return nullptr;
            }

            this->queue_->enqueue([container = navbar->getScriptContainer(), shownButton = (*window)->getShownButton()] {
                container->addChild(shownButton);
            });

            return window.release();
        }

        [[nodiscard]] as_widget_separator_t* createSeparator(
            const as_widget_base_t& ptr, const std::string& id, const std::string& label) const
        {
            return createChildWidget<gui::C_WidgetSeparator>(ptr, id, label);
        }

        [[nodiscard]] as_widget_checkbox_t* createCheckbox(
            const as_widget_base_t& ptr, const std::string& id, const std::string& label) const
        {
            return createChildWidget<gui::C_WidgetCheckbox>(ptr, id, label);
        }

        [[nodiscard]] as_widget_slider_t* createSlider(
            const as_widget_base_t& ptr, const std::string& id, int32_t min, int32_t max) const
        {
            const auto parent = resolveParent(ptr);
            if (!parent) {
                asbind20::set_script_exception("Parent must be a container!");
                return nullptr;
            }

            auto slider = createWidget<gui::C_SliderInt>(parent, id);
            if (!slider) {
                return nullptr;
            }

            slider->get()->setBounds(min, max);
            return slider.release();
        }

        [[nodiscard]] as_widget_text_t* createText(const as_widget_base_t& ptr, const std::string& label) const
        {
            return createChildWidget<gui::C_WidgetText>(ptr, std::format("as_text_id_{}", label), label);
        }

        [[nodiscard]] as_widget_same_line_t* createSameLine(const as_widget_base_t& ptr, const std::string& id) const
        {
            return createChildWidget<gui::C_WidgetSameLine>(ptr, id);
        }

        [[nodiscard]] as_widget_clr_picker_t* createColorPicker(const as_widget_base_t& ptr, const std::string& id) const
        {
            return createChildWidget<gui::C_WidgetColorPicker>(ptr, id);
        }

        std::shared_ptr<gui::C_WidgetRegedit> regedit_;
        std::shared_ptr<C_WorkerQueue> queue_;
    };
}
