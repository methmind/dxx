//
// Created by sexey on 18.02.2026.
//
module;
#include <cstdint>
#include <string_view>
#include <functional>

export module gui.widget.clickable;

import gui.widget.base;

namespace gui
{
    export class C_ClickableBase : public C_WidgetBase
    {
    public:
        using on_click_cb = std::function<void(C_ClickableBase*)>;

        ~C_ClickableBase() override = default;

        explicit C_ClickableBase(const std::string_view& id) : C_WidgetBase(id) {}

        virtual void setState(const int32_t state) {}

        [[nodiscard]] virtual int32_t getState() const { return 1; }

        void setCallback(const on_click_cb& callback) { this->onClick_ = callback; }

    protected:
        on_click_cb onClick_;

        void triggerOnClick()
        {
            if (this->onClick_) {
                this->onClick_(this);
            }
        }
    };
}
