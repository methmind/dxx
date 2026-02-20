//
// Created by sexey on 19.02.2026.
//
module;
#include <list>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

#include "nlohmann/json.hpp"

export module gui.widget.script_list;

import gui.widget.container;
import gui.widget.clickable;
import gui.widget.checkbox;

import xxhash.wrapper;

namespace gui
{
    export class C_WidgetScriptList : public C_ContainerBase
    {
    public:
        using on_click_cb = C_ClickableBase::on_click_cb;

        explicit C_WidgetScriptList(const std::string_view& id) : C_ContainerBase(id) {}

        void setCallback(const on_click_cb& callback) { this->onClick_ = callback; }

        void update(const std::list<std::filesystem::path>& collection)
        {
            std::unordered_set activePaths(collection.begin(), collection.end());

            removeStaleChildren(activePaths);
            addNewChildren(activePaths);
        }

        void render() override
        {
            if (!getVisibleState()) {
                return;
            }

            C_ContainerBase::render();
        }

        void deserialize(const nlohmann::json& document) override
        {
            const auto& children = getChildList();

            std::unordered_map<std::string, size_t, xx_hasher_s, std::equal_to<>> indexById;
            indexById.reserve(children.size());

            for (size_t i = 0; i < children.size(); ++i) {
                indexById.emplace(children[i]->getID(), i);
            }

            for (const auto& data : document) {
                const auto targetID = data.at("id").get<std::string>();
                if (const auto it = indexById.find(targetID); it != indexById.end()) {
                    children[it->second]->deserialize(data);
                }
            }
        }

        void serialize(nlohmann::json& document) override
        {
            for (const auto& child : getChildList()) {
                nlohmann::json block;
                block["id"] = child->getID();
                child->serialize(block);
                document.push_back(std::move(block));
            }
        }

    private:
        on_click_cb onClick_;

        void removeStaleChildren(std::unordered_set<std::filesystem::path>& activePaths)
        {
            for (const auto children = getChildList(); const auto& child : children) {
                auto clickable = std::dynamic_pointer_cast<C_ClickableBase>(child);
                if (!clickable) {
                    continue;
                }

                const auto it = activePaths.find(std::filesystem::path(clickable->getID()));
                if (it != activePaths.end()) {
                    activePaths.erase(it);
                    continue;
                }

                if (!clickable->getState()) {
                    removeChild(child);
                }
            }
        }

        void addNewChildren(const std::unordered_set<std::filesystem::path>& paths)
        {
            for (const auto& path : paths) {
                auto child = std::make_shared<C_WidgetCheckbox>(
                    path.string().c_str(),
                    path.filename().string().c_str()
                );

                child->setCallback(onClick_);
                addChild(child);
            }
        }
    };
}