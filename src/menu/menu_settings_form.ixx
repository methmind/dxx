//
// Created by sexey on 18.02.2026.
//
module;
#include <filesystem>
#include <list>

#include "debug/debug_output.h"
#include "nlohmann/json.hpp"

export module menu.settings;

import renderer.queue;

import gui.regedit;
import gui.widget.clickable;

import menu;
import menu.settings.designer;

import home_directory;

import as.manager;

import config;

namespace menu
{
    export constexpr auto MENU_SETTINGS_ID = "menu_settings";

    export class C_MenuSettingsForm : public C_MenuSettingsFormDesigner
    {
    public:
        ~C_MenuSettingsForm() override = default;

        explicit C_MenuSettingsForm(const std::shared_ptr<as::C_ASManager>& scriptManager,
            const std::shared_ptr<gui::C_WidgetRegedit>& widgetRegedit,
            const std::shared_ptr<render::C_RendererQueue>& renderQueue) :
            C_MenuSettingsFormDesigner(MENU_SETTINGS_ID), scriptManager_(scriptManager),
            widgetRegedit_(widgetRegedit), renderQueue_(renderQueue)
        {
            assert(scriptManager != nullptr && "AngelScript manager cant be nullptr!");
            assert(widgetRegedit != nullptr && "Widget Regedit cant be nullptr!");
            assert(renderQueue != nullptr && "Render Queue cant be nullptr!");
        }

        bool initialize() override
        {
            if (!C_MenuSettingsFormDesigner::initialize()) {
                dbg("Unable to initialize menu settings form designer!");
                return false;
            }

            if (!this->home_.initialize()) {
                dbg("Unable to initialize home directory!");
                return false;
            }

            this->scriptListUpdateButton_->setCallback([this](gui::C_ClickableBase* obj){ onScriptListRefreshButtonClick(obj); });
            this->configLoadButton_->setCallback([this](gui::C_ClickableBase* obj){ onConfigLoadButtonClick(obj); });
            this->configSaveButton_->setCallback([this](gui::C_ClickableBase* obj){ onConfigSaveButtonClick(obj); });
            this->scriptList_->setCallback([this](gui::C_ClickableBase* obj){ onScriptButtonClick(obj); });

            onScriptListRefreshButtonClick(this->scriptListUpdateButton_.get());
            onConfigLoadButtonClick(this->configLoadButton_.get());

            return true;
        }

    private:
        std::filesystem::path getConfigPath() const
        {
            std::filesystem::path path = this->home_.getPath();
            path.append("config.json");

            return path;
        }

        std::list<std::filesystem::path> getScriptList() const
        {
            std::list<std::filesystem::path> luaList;
            for (const auto& it : std::filesystem::directory_iterator(this->home_.getPath())) {
                if (!it.is_regular_file() || it.path().extension().compare(".as") != 0) {
                    continue;
                }

                luaList.push_back(it.path());
            }

            return luaList;
        }

        void deserialize(const nlohmann::json& document) override
        {
            this->humanaizerCheckbox_->deserialize(document.at(this->humanaizerCheckbox_->getID()));
            this->silentMouseCheckbox_->deserialize(document.at(this->silentMouseCheckbox_->getID()));
            this->outOfBoundsCheckbox_->deserialize(document.at(this->outOfBoundsCheckbox_->getID()));
            this->humanaizerSmooth_->deserialize(document.at(this->humanaizerSmooth_->getID()));
            this->scriptList_->deserialize(document.at(this->scriptList_->getID()));
        }

        void serialize(nlohmann::json& document) override
        {
            this->humanaizerCheckbox_->serialize(document[this->humanaizerCheckbox_->getID().c_str()]);
            this->silentMouseCheckbox_->serialize(document[this->silentMouseCheckbox_->getID().c_str()]);
            this->outOfBoundsCheckbox_->serialize(document[this->outOfBoundsCheckbox_->getID().c_str()]);
            this->humanaizerSmooth_->serialize(document[this->humanaizerSmooth_->getID().c_str()]);
            this->scriptList_->serialize(document[this->scriptList_->getID().c_str()]);
        }

        void onScriptButtonClick(gui::C_ClickableBase* obj) const
        {
            const auto scriptManager = this->scriptManager_.lock();
            if (!scriptManager) {
                dbg("Unable to get ref of angelscript manager!");
                return;
            }

            const auto objectID = obj->getID().c_str();
            if (obj->getState()) {
                if (scriptManager->isScriptLoaded(objectID)) {
                    scriptManager->disposeScript(objectID); // Reload all active scripts...
                }

                scriptManager->loadScript(objectID);
            } else {
                scriptManager->disposeScript(objectID);
            }
        }

        void onScriptListRefreshButtonClick(gui::C_ClickableBase* obj) const
        {
            this->scriptList_->update(getScriptList());
        }

        void onConfigLoadButtonClick(gui::C_ClickableBase* obj) const
        {
            const auto regedit = this->widgetRegedit_.lock();
            if (!regedit) {
                dbg("Unable to get widget regedit!");
                return;
            }

            auto config = std::make_shared<config::C_Config>();
            if (!config->initialize(getConfigPath())) {
                dbg("Unable to initialize config!");
                return;
            }

            if (!config->load(MENU_SETTINGS_ID, regedit)) {
                dbg("Unable to load config!");
                return;
            }

            this->renderQueue_->enqueue([config, regedit] {
                if (!config->loadAll(regedit)) {
                    dbg("Unable to load config!");
                }
            });
        }

        void onConfigSaveButtonClick(gui::C_ClickableBase* obj) const
        {
            const auto regedit = this->widgetRegedit_.lock();
            if (!regedit) {
                dbg("Unable to get widget regedit!");
                return;
            }

            if (!config::C_Config::Save(getConfigPath(), regedit->list())) {
                dbg("Unable to save config!");
            }
        }

        home::C_HomeDirectory home_;
        std::weak_ptr<as::C_ASManager> scriptManager_;
        std::weak_ptr<gui::C_WidgetRegedit> widgetRegedit_;
        std::shared_ptr<render::C_RendererQueue> renderQueue_;
    };
}
