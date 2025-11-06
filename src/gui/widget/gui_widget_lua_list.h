//
// Created by sexey on 03.11.2025.
//

#ifndef GUI_WIDGET_LUA_LIST_H
#define GUI_WIDGET_LUA_LIST_H

#include "gui/gui_clickable_interface.h"
#include "gui/gui_container_interface.h"

namespace gui::widget
{
    class C_WidgetLuaList : public C_IContainer, public C_IClickable
    {
    public:

        [[nodiscard]] widget_type_e getType() const override { return widget_type_e::LUA_LIST; }

        void update(const std::list<std::filesystem::path>& collection);

        void render() override;

        //todo What is performance and optimization?
        void deserialize(const nlohmann::json& document) override;

        void serialize(nlohmann::json& document) override;

        explicit C_WidgetLuaList(const std::string_view& id, const bool isVisible = true) :
           C_IWidget(id, isVisible), C_IContainer(id, isVisible), C_IClickable(id, isVisible) {}

        ~C_WidgetLuaList() override = default;
    };
} // gui

#endif //GUI_WIDGET_LUA_LIST_H
