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
import gui.widget.clickable;
import gui.widget.window;
import gui.widget.menu_item;
import gui.widget.separator;

import gui.regedit;

import menu.navbar;
import worker_queue;

namespace as
{
    constexpr auto AS_MENU_NAMESPACE_NAME = "menu";

    using C_WidgetBaseWrapper = C_SharedPtr<gui::C_WidgetBase>;

    using C_WidgetWindowWrapper = C_SharedPtr<C_ASBindingWidgetWindow>;

    using C_WidgetSeparatorWrapper = C_SharedPtr<gui::C_WidgetSeparator>;

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

            asbind20::ref_class<C_WidgetBaseWrapper>(engine, "C_WidgetBase")
                .addref(&C_WidgetBaseWrapper::addRef)
                .release(&C_WidgetBaseWrapper::release);

            asbind20::ref_class<C_WidgetWindowWrapper>(engine, "C_WidgetWindow")
                .addref(&C_WidgetWindowWrapper::addRef)
                .release(&C_WidgetWindowWrapper::release)
                .method("C_WidgetBase@ opImplCast()",
                [](C_WidgetWindowWrapper* self) {
                    return C_WidgetWindowWrapper::implicitCast<gui::C_WidgetBase>(self);
                })
                .method("imgui::ImVec2 getPosition()",
                [](const C_WidgetWindowWrapper& self) {
                    return self->getPosition();
                })
                .method("void setPosition(imgui::ImVec2)",
                [](const C_WidgetWindowWrapper& self, ImVec2 position) {
                    self->setPosition(position);
                })
                .method("imgui::ImVec2 getSize()",
                [](const C_WidgetWindowWrapper& self) {
                    return self->getSize();
                })
                .method("void setSize(imgui::ImVec2)",
                [](const C_WidgetWindowWrapper& self, ImVec2 size) {
                    self->setSize(size);
                });

            asbind20::ref_class<C_WidgetSeparatorWrapper>(engine, "C_WidgetSeparator")
                .addref(&C_WidgetSeparatorWrapper::addRef)
                .release(&C_WidgetSeparatorWrapper::release)
                .method("C_WidgetBase@ opImplCast()",
                [](C_WidgetSeparatorWrapper* self) {
                    return C_WidgetSeparatorWrapper::implicitCast<gui::C_WidgetBase>(self);
                });

            asbind20::global(engine)
                .function("menu::C_WidgetWindow@ createWindow(const string& in, const string& in)",
                    &C_ASBindingMenu::createWindow, asbind20::auxiliary(this)
                ).function("menu::C_WidgetSeparator@ createSeparator(const C_WidgetBase& in, const string& in, const string& in)",
                    &C_ASBindingMenu::createSeparator, asbind20::auxiliary(this)
                );

            engine->SetDefaultNamespace("");
            return true;
        }

    private:
        [[nodiscard]] static std::shared_ptr<gui::C_ContainerBase> GetParent(const C_WidgetBaseWrapper& ptr)
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
                        container->removeChild(widgetShared.get());
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

        [[nodiscard]] C_WidgetWindowWrapper* createWindow(const std::string& id, const std::string& title) const
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

        [[nodiscard]] C_WidgetSeparatorWrapper* createSeparator(
            const C_WidgetBaseWrapper& ptr, const std::string& id, const std::string& label) const
        {
            const auto parent = GetParent(ptr);
            if (!parent) {
                asbind20::set_script_exception("Parent must be a container!");
                return nullptr;
            }

            return createWidget<gui::C_WidgetSeparator>(parent, id, label).release();
        }

        std::shared_ptr<gui::C_WidgetRegedit> regedit_;
        std::shared_ptr<C_WorkerQueue> queue_;
    };
}
