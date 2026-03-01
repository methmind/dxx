//
// Created by sexey on 19.02.2026.
//
module;
#include <cstdint>
#include <format>
#include <functional>
#include <type_traits>
#include <string_view>

#include "imgui.h"
#include "nlohmann/json.hpp"

export module gui.widget.slider_base;

import gui.widget.base;

namespace gui
{
    export template<typename T>
    class C_SliderBase : public C_WidgetBase
    {
    public:
        using on_changed_cb = std::function<void(C_SliderBase*)>;

        ~C_SliderBase() override = default;

        explicit C_SliderBase(const std::string_view& id) : C_WidgetBase(std::format("##{}", id)), value_(0), minValue_(0), maxValue_(0)
        {
            static_assert(std::is_arithmetic_v<T>, "C_SliderBase supports only arithmetic types");
        }

        T getValue() const { return this->value_; }

        void setValue(T value) { this->value_ = value; }

        void setBounds(const T min, const T max) { this->minValue_ = min; this->maxValue_ = max; }

        void setCallback(const on_changed_cb& callback) { this->onChanged_ = callback; }

        void render() override
        {
            if (!getVisibleState()) {
                return;
            }

            T newValue = this->value_;

            ImGui::PushItemWidth(-1.0f);
            if constexpr (std::is_same_v<T, std::int32_t>) {
                ImGui::SliderInt(getID().c_str(), &newValue, this->minValue_, this->maxValue_);
            } else {
                ImGui::SliderFloat(getID().c_str(), &newValue, this->minValue_, this->maxValue_);
            }
            ImGui::PopItemWidth();

            if (newValue != this->value_) {
                this->value_ = newValue;
                triggerOnChanged();
            }
        }

        void serialize(nlohmann::json& document) override
        {
            document["value"] = getValue();
        }

        void deserialize(const nlohmann::json& document) override
        {
            setValue(document.at("value").get<T>());
        }

    private:
        void triggerOnChanged()
        {
            if (this->onChanged_) {
                this->onChanged_(this);
            }
        }

        T value_, minValue_, maxValue_;
        on_changed_cb onChanged_;
    };
}
