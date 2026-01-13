//
// Created by sexey on 21.10.2025.
//

#ifndef GUI_CONTAINER_INTERFACE_H
#define GUI_CONTAINER_INTERFACE_H

#include "gui_widget_interface.h"
#include "cs_rcu_list.h"
#include "cs_rcu_guarded.h"

namespace gui
{
    class C_IContainer : virtual public C_IWidget, public std::enable_shared_from_this<C_IContainer>
    {
    private:
        libguarded::rcu_guarded<libguarded::rcu_list<widget_ptr_t>> childList_;

    protected:

        __forceinline auto getChildList() const
        {
            return this->childList_.lock_read();
        }

    public:

        [[nodiscard]] widget_flags_e getFlags() const override { return widget_flags_e::CONTAINER; }

        void render() override
        {
            for (const auto childList = getChildList();
                const auto& child : *childList) {
                child->render();
            }
        }

        void addChild(const widget_ptr_t& child)
        {
            child->setParent(weak_from_this());

            const auto writer = this->childList_.lock_write();
            writer->push_back(child);
        }

        void removeChild(const widget_ptr_t& child)
        {
            const auto writer = this->childList_.lock_write();
            for (auto it = writer->begin(); it != writer->end(); ++it) {
                if (it->get() != child.get()) {
                    continue;
                }

                writer->erase(it);
                break;
            }
        }

        void flush()
        {
            const auto writer = this->childList_.lock_write();
            writer->clear();
        }

        explicit C_IContainer(const std::string_view& id) :
            C_IWidget(id, true) {}

        explicit C_IContainer(const std::string_view& id, const bool isVisible) :
            C_IWidget(id, isVisible) {}
    };
}

#endif //GUI_CONTAINER_INTERFACE_H