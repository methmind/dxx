//
// Created by sexey on 20.10.2025.
//

#include "gui_widget_regedit.h"

#include "gui_container_interface.h"

namespace gui
{
    widget_ptr_t C_WidgetRegedit::find(const std::string_view& id)
    {
        const auto it = this->widgets_.find(id);
        if (it == this->widgets_.end()) {
            return nullptr;
        }

        return it->second;
    }

    bool C_WidgetRegedit::add(const widget_ptr_t& ptr)
    {
        auto [it, inserted] = this->widgets_.emplace(ptr->getID(), ptr);
        return inserted;
    }

    void C_WidgetRegedit::remove(const std::string_view& id)
    {
        const auto obj = find(id);
        if (!obj) {
            return;
        }

        if (const auto parent = obj->getParent().lock(); parent) {
            std::dynamic_pointer_cast<C_IContainer>(parent)->removeChild(obj);
        }

        this->widgets_.erase(id);
    }
} // gui