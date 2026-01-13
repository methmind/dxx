//
// Created by sexey on 20.10.2025.
//

#include "gui_widget_regedit.h"
#include "gui_container_interface.h"

namespace gui
{
    bool C_WidgetRegedit::add(const widget_ptr_t& ptr)
    {
        const auto guarded = this->widgets_.lock();
        auto [it, inserted] = guarded->emplace(ptr->getID(), ptr);

        return inserted;
    }

    void C_WidgetRegedit::remove(const std::string_view& id)
    {
        const auto obj = find(id);
        if (!obj) {
            return;
        }

        const auto guarded = this->widgets_.lock();
        if (const auto parent = obj->getParent().lock(); parent) {
            std::dynamic_pointer_cast<C_IContainer>(parent)->removeChild(obj);
        }

        guarded->erase(id.data());
    }

    C_WidgetRegedit::widget_list_t C_WidgetRegedit::list() const
    {
        const auto guarded = this->widgets_.lock_shared();
        return *guarded;
    }
} // gui