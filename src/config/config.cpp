//
// Created by sexey on 04.11.2025.
//

#include <fstream>

#include "config.h"
#include "debug/debug_output.h"
#include "menu/menu_settings_form_designer.h"

namespace config
{
    void C_Config::LoadPriority(nlohmann::json& document, const gui::C_WidgetRegedit::widget_list_t& widgets)
    {
        const auto it = std::ranges::find_if(document,
            [](const nlohmann::json& item) -> bool {
                const auto widgetID = item.at("id").get<std::string_view>();
                return !widgetID.empty() && widgetID == menu::SETTINGS_FORM_ID;
            }
        );

        if (it == document.end()) {
            return;
        }

        if (const auto widget = widgets.find(it->at("id").get<std::string>()); widget != widgets.end()) {
            widget->second->deserialize(it->at("data"));
        }

        it->clear(); // So... We anyway wanna to remove this
    }

    bool C_Config::Load(const std::filesystem::path& path, const std::shared_ptr<gui::C_WidgetRegedit>& regedit)
    {
        try {
            nlohmann::json document = nlohmann::json::parse(std::fstream(path));

            LoadPriority(document, regedit->list());
            for (const auto& data : document) {
                if (data.empty()) {
                    continue;
                }

                auto widgetID = data.at("id").get<std::string_view>();
                if (widgetID.empty()) {
                    continue;
                }

                if (const auto widget = regedit->find(widgetID); widget) {
                    widget->deserialize(data.at("data"));
                }
            }

            return true;
        } catch (const std::exception& ex) {
            dbg("Critical exception: %s", ex.what());
            return false;
        }
    }

    bool C_Config::Save(const std::filesystem::path& path, const gui::C_WidgetRegedit::widget_list_t& widgets)
    {
        try {
            nlohmann::json document = nlohmann::json::array();

            for (const auto& widget : widgets | std::views::values) {
                nlohmann::json block;
                block["id"] = widget->getID();
                auto& data = block["data"] = nlohmann::json::object();

                widget->serialize(data);
                if (data.empty()) {
                    continue;
                }

                document.push_back(std::move(block));
            }

            std::ofstream configFile(path, std::ios::trunc);
            configFile << document.dump(4);
            configFile.close();

            return true;
        } catch (const std::exception& ex) {
            dbg("Critical exception: %s", ex.what());
            return false;
        }
    }
} // config