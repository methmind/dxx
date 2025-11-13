//
// Created by sexey on 21.10.2025.
//

#ifndef GUI_CONTAINER_INTERFACE_H
#define GUI_CONTAINER_INTERFACE_H

#include <list>
#include "gui_widget_interface.h"

namespace gui
{
    class C_IContainer : virtual public C_IWidget, public std::enable_shared_from_this<C_IContainer>
    {
    private:
        std::vector<widget_ptr_t> childList_;

    public:

        [[nodiscard]] widget_flags_e getFlags() const override { return widget_flags_e::CONTAINER; }

        [[nodiscard]] const std::vector<widget_ptr_t>& getChildList()
        {
            return this->childList_;
        }

        void addChild(const widget_ptr_t& child)
        {
            child->setParent(weak_from_this());

            this->childList_.push_back(child);
        }

        void removeChild(const widget_ptr_t& child)
        {
            std::erase(this->childList_, child);
        }

        void flush() { this->childList_.clear(); }

        explicit C_IContainer(const std::string_view& id) :
            C_IWidget(id, true) {}

        explicit C_IContainer(const std::string_view& id, const bool isVisible) :
            C_IWidget(id, isVisible) {}
    };
}

#endif //GUI_CONTAINER_INTERFACE_H