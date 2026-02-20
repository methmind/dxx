//
// Created by sexey on 19.02.2026.
//
module;
#include <filesystem>
#include <fstream>

#include "debug/debug_output.h"
#include "nlohmann/json.hpp"

export module config;

import gui.regedit;

namespace config
{
    export class C_Config
    {
    public:
        ~C_Config() = default;

        C_Config() = default;

        [[nodiscard]] bool initialize(const std::filesystem::path& path)
        {
            try {
                this->document_ = nlohmann::json::parse(std::ifstream(path));
            } catch (const std::exception& ex) {
                dbg("Critical exception: {}", ex.what());
                return false;
            }

            return true;
        }

        [[nodiscard]] bool load(const std::string_view& widgetID, const std::shared_ptr<gui::C_WidgetRegedit>& regedit)
        {
            try {
                const auto it = std::ranges::find_if(this->document_,
                    [widgetID](const nlohmann::json& item) {
                        return item.at("id").get<std::string_view>() == widgetID;
                    }
                );

                if (it == this->document_.end()) {
                    return false;
                }

                DeserializeWidget(*it, regedit->list());
                this->document_.erase(it);
            } catch (const std::exception& ex) {
                dbg("Critical exception: {}", ex.what());
                return false;
            }

            return true;
        }

        [[nodiscard]] bool loadAll(const std::shared_ptr<gui::C_WidgetRegedit>& regedit)
        {
            try {
                const auto widgets = regedit->list();
                for (const auto& entry : this->document_) {
                    DeserializeWidget(entry, widgets);
                }

                return true;
            } catch (std::exception& ex) {
                dbg("Critical exception: {}", ex.what());
                return false;
            }
        }

        [[nodiscard]] static bool Save(const std::filesystem::path& path, const gui::widget_list_t& widgets)
        {
            try {
                auto document = nlohmann::json::array();

                for (const auto& weakWidget : widgets | std::views::values) {
                    const auto widget = weakWidget.lock();
                    if (!widget) {
                        return false;
                    }

                    auto entry = nlohmann::json::object({{"id", widget->getID()}});
                    auto& data = entry["data"] = nlohmann::json::object();

                    widget->serialize(data);
                    if (data.empty()) {
                        continue;
                    }

                    document.push_back(std::move(entry));
                }

                std::ofstream file(path, std::ios::trunc);
                file << document.dump(4);

                return true;
            } catch (const std::exception& ex) {
                dbg("Critical exception: {}", ex.what());
                return false;
            }
        }

    private:
        static bool DeserializeWidget(const nlohmann::json& entry, const gui::widget_list_t& widgets)
        {
            const auto widgetID = entry.at("id").get<std::string>();
            if (widgetID.empty()) {
                return false;
            }

            const auto it = widgets.find(widgetID);
            if (it == widgets.end()) {
                return false;
            }

            const auto widget = it->second.lock();
            if (!widget) {
                return false;
            }

            widget->deserialize(entry.at("data"));
            return true;
        }

        nlohmann::json document_;
    };
}