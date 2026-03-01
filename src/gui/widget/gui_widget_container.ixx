//
// Created by sexey on 17.02.2026.
//
module;
#include <algorithm>
#include <memory>
#include <string_view>
#include <vector>

export module gui.widget.container;

import gui.widget.base;
import gui.widget.type;

namespace gui
{
    export class C_ContainerBase : public C_WidgetBase
    {
    public:
        explicit C_ContainerBase(const std::string_view& id) : C_WidgetBase(id) {}

        [[nodiscard]] widget_flags_e getFlags() const override { return widget_flags_e::CONTAINER; }

        void render() override
        {
            if (!getVisibleState()) {
                return;
            }

            for (const auto& child : this->children_) {
                child->render();
            }
        }

        void addChild(const widget_ptr_t& child)
        {
            child->setParent(weak_from_this());
            this->children_.emplace_back(child);
        }

        void removeChild(const C_WidgetBase* child)
        {
            const auto it = std::ranges::remove_if(this->children_,
               [&](const widget_ptr_t& ptr) {
                   return ptr.get() == child;
               }
            ).begin();

            if (it != this->children_.end()) {
                 this->children_.erase(it, this->children_.end());
            }
        }

        __attribute__((always_inline)) void removeChild(const widget_ptr_t& child)
        {
            removeChild(child.get());
        }

    protected:
        [[nodiscard]] auto& getChildList() const { return this->children_; }

    private:
        std::vector<widget_ptr_t> children_;
    };
}