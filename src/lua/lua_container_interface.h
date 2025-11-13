//
// Created by sexey on 30.10.2025.
//

#ifndef LUA_CONTAINER_INTERFACE_H
#define LUA_CONTAINER_INTERFACE_H

#include "gui/gui_widget_interface.h"

namespace lua
{
    class C_ILuaContainer
    {
    public:

        virtual bool bindGuiWidget(const std::string_view& luaID, const gui::widget_ptr_t& widget) = 0;

        virtual ~C_ILuaContainer() = default;
    };
}

#endif //LUA_CONTAINER_INTERFACE_H