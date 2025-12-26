//
// Created by sexey on 03.11.2025.
//

#ifndef GUI_WIDGET_SLIDER_INTERFACE_H
#define GUI_WIDGET_SLIDER_INTERFACE_H

#include <format>

#include "gui_widget_interface.h"
#include "imgui.h"

namespace gui
{
    template <typename T>
    class C_ISlider : virtual public C_IWidget
    {
    public:
        using on_changed_cb = std::function<void(C_ISlider*)>;

    private:
        T value_, minValue_, maxValue_;
        on_changed_cb onChanged_;

        void triggerOnChanged()
        {
            if (this->onChanged_) {
                this->onChanged_(this);
            }
        }

    public:

        [[nodiscard]] widget_type_e getType() const override
        {
            if constexpr (std::is_integral_v<T>) {
                return widget_type_e::SLIDER_INT;
            } else if constexpr (std::is_floating_point_v<T>) {
                return widget_type_e::SLIDER_FLOAT;
            } else {
                static_assert(std::is_arithmetic_v<T>, "C_ISlider supports only arithmetic types");
                return widget_type_e::SLIDER;
            }
        }

        T getValue() const { return this->value_; }

        void setValue(const T newValue)
        {
            this->value_ = newValue;
        }

        void setBounds(const T minValue, const T maxValue)
        {
            this->minValue_ = minValue;
            this->maxValue_ = maxValue;
        }

        void setCallback(on_changed_cb onChanged)
        {
            this->onChanged_ = std::move(onChanged);
        }

        void render() override
        {
            if (!isVisible()) {
                return;
            }

            const auto rect = ImGui::GetContentRegionAvail();
            T newValue = this->value_;

            ImGui::PushItemWidth(rect.x);
            if constexpr (std::is_same_v<T, int32_t>) {
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

        explicit C_ISlider(const std::string_view& id) :
            C_IWidget(std::format("##{}", id), true),
            value_(0), minValue_(0), maxValue_(0), onChanged_(nullptr) {}

        explicit C_ISlider(const std::string_view& id, const bool isVisible) :
            C_IWidget(std::format("##{}", id), isVisible),
            value_(0), minValue_(0), maxValue_(0), onChanged_(nullptr) {}
    };
}

#endif //GUI_WIDGET_SLIDER_INTERFACE_H