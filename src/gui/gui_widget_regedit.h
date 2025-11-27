//
// Created by sexey on 20.10.2025.
//

#ifndef GUI_WIDGET_REGEDIT_H
#define GUI_WIDGET_REGEDIT_H

#include <string_view>
#include <unordered_map>

#include "cs_shared_guarded.h"
#include "gui_widget_interface.h"
#include "hash/xxhash_wrapper.h"

namespace gui
{
    class C_WidgetRegedit
    {
    public:
        using widget_list_t = std::unordered_map<std::string, widget_ptr_t, xx_hashier_s>;

    private:
        libguarded::shared_guarded<widget_list_t> widgets_;

    public:

        widget_ptr_t find(const std::string_view& id) const;

        bool add(const widget_ptr_t& ptr);

        void remove(const std::string_view& id);

        widget_list_t list() const;

        template<typename widget_t, typename ... args_t>
        std::shared_ptr<widget_t> createWidget(args_t&& ... args)
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