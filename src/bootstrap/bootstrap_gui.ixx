//
// Created by sexey on 17.02.2026.
//
module;
#include <cassert>
#include <memory>

#include "imgui.h"
#include "MinHook.h"
#include "debug/debug_output.h"

export module bootstrap.gui;

import service.container;
import service.locator;

import renderer.queue;

import gui.regedit;
import gui.widget.root;
import gui.widget.clickable;
import gui.widget.menu_item;

import menu.navbar;
import menu.settings;

import hook.dispatcher;
import hook.type;

import as.manager;

import bind_system;

namespace bootstrap
{
    struct render_hook_subscription_proxy_s : hook::hook_subscription_t {};

    struct keybind_subscription_proxy_s : input::bind_subscription_t {};

    export bool InitializeGUI(const std::unique_ptr<C_ServiceContainer>& services)
    {
        const auto renderQueue = services->add<render::C_RendererQueue>();;
        const auto widgetRegedit = services->add<gui::C_WidgetRegedit>();
        const auto guiRoot = widgetRegedit->createWidget<gui::C_WidgetRoot>();

        auto navbarForm = widgetRegedit->createWidget<menu::C_MenuNavbarForm>();
        if (!navbarForm->initialize()) {
            dbg("Unable to initialize navbar form!");
            return false;
        }

        auto settingsForm = widgetRegedit->createWidget<menu::C_MenuSettingsForm>(
            services->get<as::C_ASManager>(),
            widgetRegedit,
            renderQueue
        );

        if (!settingsForm->initialize()) {
            dbg("Unable to initialize settings form!");
            return false;
        }

        const auto settingsButton = std::make_shared<gui::C_WidgetMenuItem>("navbar_setting_button", "Settings");
        settingsButton->setCallback([settingsForm](gui::C_ClickableBase* self) {
            settingsForm->setVisibleState(true);
        });

        navbarForm->getSystemContainer()->addChild(settingsButton);
        navbarForm->addChild(settingsForm);
        guiRoot->addChild(navbarForm);

        services->add<render_hook_subscription_proxy_s>(
            C_ServiceLocator::Get<hook::C_HookDispatcher>()->subscribe<ImDrawList*>(static_cast<uint16_t>(hook::hook_type_e::ON_IMGUI_RENDER),
                [renderQueue, guiRoot](ImDrawList* ctx) {
                    guiRoot->render();
                    renderQueue->processCommands();
                }
            )
        );

        const auto bindService = services->get<input::C_BindSystem>();
        assert(bindService != nullptr && "input::C_BindSystem service is not registered!");

        services->add<keybind_subscription_proxy_s>(
            bindService->bind(VK_INSERT, [navbarForm] {
                navbarForm->setVisibleState(!navbarForm->getVisibleState());
            })
        );

        return true;
    }
}