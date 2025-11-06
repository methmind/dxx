//
// Created by sexey on 22.10.2025.
//

#ifndef GUI_BUTTON_INTERFACE_H
#define GUI_BUTTON_INTERFACE_H

#include <functional>
#include <utility>

#include "gui_widget_interface.h"

namespace gui
{
    class C_IClickable;

    using on_click_cb = std::function<void(C_IClickable*)>;

    class C_IClickable : virtual public C_IWidget
    {
    protected:
        on_click_cb onClick_;

        void triggerOnClick()
        {
            if (this->onClick_) {
                this->onClick_(this);
            }
        }

    public:

        virtual void setState(const int32_t state) {}

        [[nodiscard]] virtual int32_t getState() const { return 1; }

        void setCallback(on_click_cb callback) { this->onClick_ = std::move(callback); }

        explicit C_IClickable(const std::string_view& id) :
            C_IWidget(id, true), onClick_(nullptr) {}

        explicit C_IClickable(const std::string_view& id, const bool isVisible) :
            C_IWidget(id, isVisible), onClick_(nullptr) {}
    };
}

#endif //GUI_BUTTON_INTERFACE_H