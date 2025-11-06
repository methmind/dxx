//
// Created by sexey on 23.10.2025.
//

#include <fstream>

#include "menu_settings_form.h"

#include "config/config.h"
#include "debug/debug_output.h"

namespace menu
{
    std::filesystem::path C_MenuSettingsForm::getConfigPath() const
    {
        std::filesystem::path configPath = this->home_.getPath();
        configPath.append("config.json");

        return configPath;
    }

    std::list<std::filesystem::path> C_MenuSettingsForm::getLuaList() const
    {
        std::list<std::filesystem::path> luaList;
        for (const auto& it : std::filesystem::directory_iterator(this->home_.getPath())) {
            if (!it.is_regular_file() || it.path().extension().compare(LUA_FILE_EXTENSION) != 0) {
                continue;
            }

            luaList.push_back(it.path());
        }

        return luaList;
    }

    void C_MenuSettingsForm::deserialize(const nlohmann::json& document)
    {
        this->humanaizerCheckbox_->deserialize(document.at(this->humanaizerCheckbox_->getID()));
        this->silentMouseCheckbox_->deserialize(document.at(this->silentMouseCheckbox_->getID()));
        this->debugHumaizerCheckbox_->deserialize(document.at(this->debugHumaizerCheckbox_->getID()));
        this->outOfBoundsCheckbox_->deserialize(document.at(this->outOfBoundsCheckbox_->getID()));
        this->humanaizerSmooth_->deserialize(document.at(this->humanaizerSmooth_->getID()));
        this->luaList_->deserialize(document.at(this->luaList_->getID()));
    }

    void C_MenuSettingsForm::serialize(nlohmann::json& document)
    {
        this->humanaizerCheckbox_->serialize(document[this->humanaizerCheckbox_->getID().c_str()]);
        this->silentMouseCheckbox_->serialize(document[this->silentMouseCheckbox_->getID().c_str()]);
        this->debugHumaizerCheckbox_->serialize(document[this->debugHumaizerCheckbox_->getID().c_str()]);
        this->outOfBoundsCheckbox_->serialize(document[this->outOfBoundsCheckbox_->getID().c_str()]);
        this->humanaizerSmooth_->serialize(document[this->humanaizerSmooth_->getID().c_str()]);
        this->luaList_->serialize(document[this->luaList_->getID().c_str()]);
    }

    void C_MenuSettingsForm::onLuaRefreshListButtonClick(gui::C_IClickable* obj)
    {
        this->luaList_->update(getLuaList());
    }

    void C_MenuSettingsForm::onLuaButtonClick(gui::C_IClickable* obj)
    {
        const auto objectID = obj->getID().c_str();
        if (obj->getState()) {
            if (this->lua_->isScriptLoaded(objectID)) {
                this->lua_->disposeScript(objectID); // Reload all active scripts...
            }

            this->lua_->loadScript(objectID);
        } else {
            this->lua_->disposeScript(objectID);
        }
    }

    void C_MenuSettingsForm::onConfigLoadButtonClick(gui::C_IClickable* obj)
    {
        if (!config::C_Config::Load(getConfigPath(), this->widgetRegedit_.lock()->list())) {
            dbg("Unable to load config!");
        }
    }

    void C_MenuSettingsForm::onConfigSaveButtonClick(gui::C_IClickable* obj)
    {
        if (!config::C_Config::Save(getConfigPath(), this->widgetRegedit_.lock()->list())) {
            dbg("Unable to save config!");
        }
    }

    bool C_MenuSettingsForm::initialize()
    {
        if (!this->home_.initialize()) {
            dbg("Unable to initialize home directory!");
            return false;
        }

        if (!C_MenuSettingsFormDesigner::initialize()) {
            dbg("Unable to initialize menu settings form designer!");
            return false;
        }

        onLuaRefreshListButtonClick(this->luaListUpdateButton_.get());
        onConfigLoadButtonClick(this->configLoadButton_.get()); // load default config...

        return true;
    }
} // menu