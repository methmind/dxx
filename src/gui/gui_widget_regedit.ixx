//
// Created by sexey on 17.02.2026.
//
module;
#include <unordered_map>

#include "cs_shared_guarded.h"

export module gui.regedit;

import xxhash.wrapper;
import gui.widget.base;
import gui.widget.container;

namespace gui
{
    export using widget_list_t = std::unordered_map<std::string, weak_widget_ptr_t, xx_hasher_s, std::equal_to<>>;

    using shared_list_t = libguarded::shared_guarded<widget_list_t>;

    template <typename T>
    concept widget_t = std::derived_from<T, C_WidgetBase>;

    export class C_WidgetRegedit
    {
    public:
        ~C_WidgetRegedit() = default;

        C_WidgetRegedit() = default;

        C_WidgetRegedit(const C_WidgetRegedit&) = delete;

        C_WidgetRegedit& operator=(const C_WidgetRegedit&) = delete;

        template<widget_t T = C_WidgetBase>
        std::shared_ptr<T> find(const std::string_view& id) const
        {
            const auto guarded = this->widgets_.lock_shared();

            const auto it = guarded->find(id.data());
            if (it == guarded->end()) {
                return nullptr;
            }

            if constexpr (std::is_same_v<T, C_WidgetBase>) {
                return it->second.lock();
            } else {
                return std::dynamic_pointer_cast<T>(it->second.lock());
            }
        }

        bool add(const widget_ptr_t& ptr)
        {
            const auto guarded = this->widgets_.lock();
            auto [it, inserted] = guarded->emplace(ptr->getID(), ptr);

            return inserted;
        }

        void remove(const std::string_view& id)
        {
            this->widgets_.lock()->erase(id.data());
        }

        widget_list_t list() const
        {
            const auto guarded = this->widgets_.lock_shared();
            return *guarded;
        }

        template<typename T, typename ... args_t>
        std::shared_ptr<T> createWidget(args_t&& ... args)
        {
            auto widget = std::make_shared<T>(std::forward<args_t>(args)...);
            if (!add(widget)) {
                return nullptr;
            }

            return widget;
        }

    private:
        shared_list_t widgets_;
    };
}