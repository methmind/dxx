//
// Created by sexey on 17.02.2026.
//
module;

export module gui.widget.type;

export namespace gui
{
     enum class widget_flags_e
     {
         DEFAULT = 0,
         CONTAINER = 1 << 0,
         CLICKABLE = 1 << 1
     };
}