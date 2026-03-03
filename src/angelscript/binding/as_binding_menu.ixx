//
// Created by sexey on 28.02.2026.
//
module;
#include <format>
#include <memory>

#include "imgui.h"
#include "asbind20/asbind.hpp"
#include "scriptarray/scriptarray.h"

export module as.binding.menu;

import as.binding;
import as.binding.memory;
import as.engine_interface;
import as.binding.widget_window;
import as.binding.d3d_texture;

import gui.widget.base;
import gui.widget.container;
import gui.widget.window;
import gui.widget.separator;
import gui.widget.checkbox;
import gui.widget.slider;
import gui.widget.text;
import gui.widget.same_line;
import gui.widget.color_picker;
import gui.widget.combobox;
import gui.widget.image_checkbox;
import gui.widget.header;

import gui.regedit;

import menu.navbar;
import worker_queue;

namespace as
{
    constexpr auto AS_MENU_NAMESPACE_NAME = "menu";

    using as_widget_base_t           = C_SharedPtr<gui::C_WidgetBase>;
    using as_widget_window_t         = C_SharedPtr<C_ASBindingWidgetWindow>;
    using as_widget_separator_t      = C_SharedPtr<gui::C_WidgetSeparator>;
    using as_widget_checkbox_t       = C_SharedPtr<gui::C_WidgetCheckbox>;
    using as_widget_slider_t         = C_SharedPtr<gui::C_SliderInt>;
    using as_widget_text_t           = C_SharedPtr<gui::C_WidgetText>;
    using as_widget_same_line_t      = C_SharedPtr<gui::C_WidgetSameLine>;
    using as_widget_clr_picker_t     = C_SharedPtr<gui::C_WidgetColorPicker>;
    using as_widget_combobox_t       = C_SharedPtr<gui::C_WidgetCombobox>;
    using as_widget_image_checkbox_t = C_SharedPtr<gui::C_WidgetImageCheckbox>;
    using as_widget_header_t         = C_SharedPtr<gui::C_WidgetHeader>;

    // Регистрирует ref-тип с upcast к C_WidgetBase.
    template<typename wrapper_t>
    auto registerWidgetType(asIScriptEngine* engine, const char* name)
    {
        auto binder = asbind20::ref_class<wrapper_t>(engine, name)
            .addref(&wrapper_t::addRef)
            .release(&wrapper_t::release);

        binder.method(
            "C_WidgetBase@ opImplCast()",
            [](wrapper_t* self) -> as_widget_base_t* {
                return wrapper_t::template implicitCast<gui::C_WidgetBase>(self);
            }
        );

        return binder;
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
            asbind20::ref_class<as_widget_base_t>(engine, "C_WidgetBase")
                .addref(&as_widget_base_t::addRef)
                .release(&as_widget_base_t::release);

            registerWidgetType<as_widget_window_t>(engine, "C_WidgetWindow")
                .method("imgui::ImVec2 getPosition()",
                    [](const as_widget_window_t& self) { return self->getPosition(); })
                .method("void setPosition(imgui::ImVec2)",
                    [](const as_widget_window_t& self, ImVec2 pos) { self->setPosition(pos); })
                .method("imgui::ImVec2 getSize()",
                    [](const as_widget_window_t& self) { return self->getSize(); })
                .method("void setSize(imgui::ImVec2)",
                    [](const as_widget_window_t& self, ImVec2 size) { self->setSize(size); });

            registerWidgetType<as_widget_separator_t>(engine, "C_WidgetSeparator");

            registerWidgetType<as_widget_checkbox_t>(engine, "C_WidgetCheckbox")
                .method("bool getState()",
                    [](const as_widget_checkbox_t& self) { return self->getState(); })
                .method("void setState(bool)",
                    [](const as_widget_checkbox_t& self, bool state) { self->setState(state); });

            registerWidgetType<as_widget_slider_t>(engine, "C_WidgetSlider")
                .method("int getValue()",
                    [](const as_widget_slider_t& self) { return self->getValue(); })
                .method("void setValue(int)",
                    [](const as_widget_slider_t& self, int32_t value) { self->setValue(value); });

            registerWidgetType<as_widget_text_t>(engine, "C_WidgetText");

            registerWidgetType<as_widget_same_line_t>(engine, "C_WidgetSameLine");

            registerWidgetType<as_widget_clr_picker_t>(engine, "C_WidgetColorPicker")
                .method("imgui::ImVec4 getColor()",
                    [](const as_widget_clr_picker_t& self) { return self->getColor(); })
                .method("void setColor(imgui::ImVec4)",
                    [](const as_widget_clr_picker_t& self, ImVec4 color) { self->setColor(color); });

            registerWidgetType<as_widget_combobox_t>(engine, "C_WidgetCombobox")
                .method("void setSelectedIndex(int)",
                    [](const as_widget_combobox_t& self, int32_t selected) { self->setSelectedIndex(selected); })
                .method("int getSelectedIndex()",
                    [](const as_widget_combobox_t& self) { return self->getSelectedIndex(); })
                .method("void setOptions(const array<string>& in)", [](const as_widget_combobox_t& self, CScriptArray* array) {
                    if (!array) {
                        asbind20::set_script_exception("Options array cannot be null!");
                        return;
                    }

                    std::vector<std::string> tmp(array->GetSize());
                    for (asUINT i = 0; i < array->GetSize(); i++) {
                        tmp[i] = *static_cast<std::string*>(array->At(i));
                    }

                    self->setOptions(tmp);
                });

            registerWidgetType<as_widget_image_checkbox_t>(engine, "C_WidgetImageCheckbox")
                .method("bool getState()",
                    [](const as_widget_image_checkbox_t& self) { return self->getState(); })
                .method("void setState(bool)",
                    [](const as_widget_image_checkbox_t& self, bool state) { self->setState(state); });

            registerWidgetType<as_widget_header_t>(engine, "C_WidgetHeader");
        }

        void registerFactoryFunctions(asIScriptEngine* engine)
        {
            asbind20::global(engine)
                .function("bool isWidgetVisible(const C_WidgetBase& in)",
                    [](const as_widget_base_t& ptr) { return ptr->getVisibleState(); })
                .function("void setWidgetVisible(bool)",
                    [](const as_widget_base_t& ptr, bool visible) { ptr->setVisibleState(visible); })
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
                    &C_ASBindingMenu::createColorPicker, asbind20::auxiliary(this))
                .function("menu::C_WidgetCombobox@ createCombobox(const C_WidgetBase& in, const string& in)",
                    &C_ASBindingMenu::createCombobox, asbind20::auxiliary(this))
                .function("menu::C_WidgetImageCheckbox@ createImageCheckbox(const C_WidgetBase& in, const string& in, const render::d3d_texture_t& in, imgui::ImVec2)",
                    &C_ASBindingMenu::createImageCheckbox, asbind20::auxiliary(this))
                .function("menu::C_WidgetHeader@ createHeader(const C_WidgetBase& in, const string& in, const string& in)",
                    &C_ASBindingMenu::createHeader, asbind20::auxiliary(this));
        }

        [[nodiscard]] static std::shared_ptr<gui::C_ContainerBase> resolveParent(const as_widget_base_t& ptr)
        {
            if (!ptr) {
                return nullptr;
            }

            return std::dynamic_pointer_cast<gui::C_ContainerBase>(ptr.get_shared());
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

            // Токен захватывает widget по shared_ptr — виджет гарантированно жив при dispose.
            // Токен разделен между оригиналом и всеми кастами (implicitCast).
            auto token = std::make_shared<dispose_token_s>(
                [regedit = this->regedit_, queue = this->queue_, widget] {
                    queue->enqueue([widget] {
                        const auto parent = widget->getParent().lock();
                        if (!parent) {
                            return;
                        }

                        if (auto container = std::dynamic_pointer_cast<gui::C_ContainerBase>(parent)) {
                            container->removeChild(widget);
                        }
                    });

                    regedit->remove(widget->getID());
                }
            );

            return std::make_unique<C_SharedPtr<T>>(widget, std::move(token));
        }

        // Общий паттерн для создания дочернего виджета: resolveParent → createWidget → release.
        template<typename T, typename... args_t>
        [[nodiscard]] C_SharedPtr<T>* createChildWidget(
            const as_widget_base_t& parentRef, args_t&&... args) const
        {
            const auto parent = resolveParent(parentRef);
            auto result = createWidget<T>(parent, std::forward<args_t>(args)...);
            if (!result) {
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

            (*window)->initialize();
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
            const auto slider = createChildWidget<gui::C_SliderInt>(ptr, id);
            if (!slider) {
                return nullptr;
            }

            (*slider)->setBounds(min, max);
            return slider;
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

        [[nodiscard]] as_widget_combobox_t* createCombobox(const as_widget_base_t& ptr, const std::string& id) const
        {
            return createChildWidget<gui::C_WidgetCombobox>(ptr, id);
        }

        [[nodiscard]] as_widget_image_checkbox_t* createImageCheckbox(const as_widget_base_t& ptr, const std::string& id,
            const d3d_texture_wrapper_t& texture, ImVec2 textureSize) const
        {
            const auto checkbox = createChildWidget<gui::C_WidgetImageCheckbox>(ptr, id);
            if (!checkbox) {
                return nullptr;
            }

            const auto widget = checkbox->get();
            widget->setTexture(texture.get_shared());
            widget->setTextureSize(textureSize);

            return checkbox;
        }

        [[nodiscard]] as_widget_header_t* createHeader(const as_widget_base_t& ptr, const std::string& id, const std::string& label) const
        {
            return createChildWidget<gui::C_WidgetHeader>(ptr, id, label);
        }

        std::shared_ptr<gui::C_WidgetRegedit> regedit_;
        std::shared_ptr<C_WorkerQueue> queue_;
    };
}
