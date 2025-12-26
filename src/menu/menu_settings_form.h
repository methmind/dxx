//
// Created by sexey on 23.10.2025.
//

#ifndef MENU_SETTINGS_FORM_H
#define MENU_SETTINGS_FORM_H

#include "menu_settings_form_designer.h"
#include "gui/gui_widget_regedit.h"
#include "home/home_directory.h"
#include "lua/lua_script_manager.h"

namespace menu
{
    constexpr auto CFG_WIDGETS_FIELD = "widgets";
    const std::filesystem::path LUA_FILE_EXTENSION = ".lua";

    class C_MenuSettingsForm final : public C_MenuSettingsFormDesigner
    {
    private:
        home::C_HomeDirectory home_;
        std::weak_ptr<gui::C_WidgetRegedit> widgetRegedit_;
        std::weak_ptr<lua::C_LuaScriptManager> lua_;

        std::filesystem::path getConfigPath() const;

        std::list<std::filesystem::path> getLuaList() const;

        void deserialize(const nlohmann::json& document) override;

        void serialize(nlohmann::json& document) override;

        void onLuaRefreshListButtonClick(gui::C_IClickable* obj) override;

        void onLuaButtonClick(gui::C_IClickable* obj) override;

        void onConfigLoadButtonClick(gui::C_IClickable* obj) override;

        void onConfigSaveButtonClick(gui::C_IClickable* obj) override;

    public:

        bool initialize() override;

        void setVisibleState(const bool isVisible) override {
            this->settingsWindow_->setVisibleState(isVisible);
        }

        explicit C_MenuSettingsForm(
            const std::weak_ptr<gui::C_WidgetRegedit>& widgetRegedit,
            const std::weak_ptr<lua::C_LuaScriptManager>& luaManager
        ) :
            C_IWidget(SETTINGS_FORM_ID), widgetRegedit_(widgetRegedit), lua_(luaManager) {}

        ~C_MenuSettingsForm() override = default;
    };
} // menu

#endif //MENU_SETTINGS_FORM_H