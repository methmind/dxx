//
// Created by sexey on 20.10.2025.
//

#ifndef GUI_WIDGET_REGEDIT_H
#define GUI_WIDGET_REGEDIT_H

#include <memory>
#include <string_view>
#include <unordered_map>

#include "gui_widget_interface.h"

namespace gui
{
    class C_WidgetRegedit
    {
    private:
        std::unordered_map<std::string_view, widget_ptr_t> widgets_;

    public:

        using widget_list_t = decltype(widgets_);

        widget_ptr_t find(const std::string_view& id);

        bool add(const widget_ptr_t& ptr);

        void remove(const std::string_view& id);

        widget_list_t list() const { return this->widgets_; }

        template<typename widget_t, typename ... args_t>
        widget_ptr_t createWidget(args_t&& ... args)
        {
            auto widget = std::make_shared<widget_t>(std::forward<args_t>(args)...);
            if (!add(widget)) {
                return nullptr;
            }

            return widget;
        }

        C_WidgetRegedit() = default;

        ~C_WidgetRegedit() = default;
    };
} // gui

#endif //GUI_WIDGET_REGEDIT_H