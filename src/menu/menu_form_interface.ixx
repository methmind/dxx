//
// Created by sexey on 17.02.2026.
//
module;
#include <string_view>

export module menu;

import gui.widget.container;

namespace menu
{
    export class C_IForm : public gui::C_ContainerBase
    {
    public:
        virtual bool initialize() = 0;

        explicit C_IForm(const std::string_view& id) : C_ContainerBase(id) {}
    };
}