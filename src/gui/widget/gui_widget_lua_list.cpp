//
// Created by sexey on 03.11.2025.
//

#include <unordered_set>

#include "gui_widget_lua_list.h"
#include "gui_widget_checkbox.h"

namespace gui::widget
{
    //todo What is performance and optimization?
    void C_WidgetLuaList::update(const std::list<std::filesystem::path>& collection)
    {
        std::unordered_set<std::filesystem::path> newPaths;
        for (const auto& file : collection) {
            newPaths.emplace(file);
        }

        for (const auto tmpList = getChildList(); const auto& c : *tmpList) {
            const auto child = std::dynamic_pointer_cast<C_IClickable>(c);
            if (!child) {
                continue;
            }

            const auto it = newPaths.find(std::filesystem::path(child->getID()));
            if (it == newPaths.end()) {
                if (!child->getState()) {
                    removeChild(child); // inactive lua script
                }

                continue;
            }

            newPaths.erase(it);
        }

        for (const auto& path : newPaths) {
            auto newChild = std::make_shared<C_WidgetCheckbox>(path.string().c_str(),
                path.filename().string().c_str()
            );

            newChild->setCallback(this->onClick_);
            addChild(newChild);
        }
    }

    void C_WidgetLuaList::render()
    {
        if (!isVisible()) {
            return;
        }

        C_IContainer::render();
    }

    //todo What is performance and optimization?
    void C_WidgetLuaList::deserialize(const nlohmann::json& document)
    {
        const auto childVector = getChildList();

        for (auto& data : document) {
            const auto targetID = data.at("id").get<std::string_view>();
            for (const auto& it : *childVector) {
                if (it->getID() != targetID.data()) {
                    continue;
                }

                it->deserialize(data);
                break;
            }
        }
    }

    void C_WidgetLuaList::serialize(nlohmann::json& document)
    {
        for (const auto childVector = getChildList(); const auto& child : *childVector) {
            nlohmann::json block;
            block["id"] = child->getID();
            child->serialize(block);

            document.push_back(block);
        }
    }
} // gui