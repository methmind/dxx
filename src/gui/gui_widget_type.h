//
// Created by sexey on 28.10.2025.
//

#ifndef GUI_WIDGET_TYPE_H
#define GUI_WIDGET_TYPE_H

#include <type_traits>
#include <cstdint>

namespace gui
{
    enum class widget_type_e : uint8_t
    {
        ROOT,
        NAVBAR,
        WINDOW,
        TEXT,
        SLIDER,
        SLIDER_INT,
        SLIDER_FLOAT,
        SEPARATOR,
        SAME_LINE,
        MENU_ITEM,
        CONTEXT_MENU,
        CHILD_WINDOW,
        CHECKBOX,
        WINDOWS_CONTAINER,
        BUTTON,
        LUA_LIST
    };

    enum class widget_flags_e : uint16_t
    {
        DEFAULT = 0,
        CONTAINER = 1 << 0,
        CLICKABLE = 1 << 1,
    };

    inline constexpr widget_flags_e operator|(widget_flags_e lhs, widget_flags_e rhs) {
        return static_cast<widget_flags_e>(
            static_cast<std::underlying_type_t<widget_flags_e>>(lhs) |
            static_cast<std::underlying_type_t<widget_flags_e>>(rhs)
        );
    }

    inline constexpr widget_flags_e operator&(widget_flags_e lhs, widget_flags_e rhs) {
        return static_cast<widget_flags_e>(
            static_cast<std::underlying_type_t<widget_flags_e>>(lhs) &
            static_cast<std::underlying_type_t<widget_flags_e>>(rhs)
        );
    }

    inline constexpr widget_flags_e operator^(widget_flags_e lhs, widget_flags_e rhs) {
        return static_cast<widget_flags_e>(
            static_cast<std::underlying_type_t<widget_flags_e>>(lhs) ^
            static_cast<std::underlying_type_t<widget_flags_e>>(rhs)
        );
    }

    inline constexpr widget_flags_e operator~(widget_flags_e e) {
        return static_cast<widget_flags_e>(
            ~static_cast<std::underlying_type_t<widget_flags_e>>(e)
        );
    }

    inline widget_flags_e& operator|=(widget_flags_e& lhs, widget_flags_e rhs) {
        return lhs = lhs | rhs;
    }

    inline widget_flags_e& operator&=(widget_flags_e& lhs, widget_flags_e rhs) {
        return lhs = lhs & rhs;
    }

    inline widget_flags_e& operator^=(widget_flags_e& lhs, widget_flags_e rhs) {
        return lhs = lhs ^ rhs;
    }

    inline constexpr bool operator!(widget_flags_e e) {
        return static_cast<std::underlying_type_t<widget_flags_e>>(e) == 0;
    }
}

#endif //GUI_WIDGET_TYPE_H