//
// Created by sexey on 17.02.2026.
//
module;

export module gui.widget.root;

import gui.widget.container;

export namespace gui
{
    constexpr auto ROOT_WIDGET_ID = "root";

    class C_WidgetRoot final : public C_ContainerBase
    {
    public:
        explicit C_WidgetRoot() : C_ContainerBase(ROOT_WIDGET_ID) {}
    };
}