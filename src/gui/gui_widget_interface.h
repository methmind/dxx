//
// Created by sexey on 20.10.2025.
//

#ifndef GUI_WIDGET_INTERFACE_H
#define GUI_WIDGET_INTERFACE_H

#include <memory>
#include <string>
#include "gui_widget_type.h"
#include "nlohmann/json.hpp"

namespace gui
{
    class C_IWidget
    {
    private:
        std::string id_;
        bool isVisible_;
        std::weak_ptr<C_IWidget> parent_;

    public:

        virtual void* metacast(const widget_flags_e type)
        {
            if (type == widget_flags_e::DEFAULT) {
                return this;
            }

            return nullptr;
        }

        void setParent(const std::weak_ptr<C_IWidget>& parent) { this->parent_ = parent; }

        [[nodiscard]] std::weak_ptr<C_IWidget> getParent() const { return this->parent_; }

        [[nodiscard]] virtual widget_flags_e getFlags() const { return widget_flags_e::DEFAULT; }

        [[nodiscard]] virtual widget_type_e getType() const { return widget_type_e::ROOT; }

        virtual void setVisibleState(const bool isVisible) {
            this->isVisible_ = isVisible;
        }

        [[nodiscard]] bool isVisible() const {
            return this->isVisible_;
        }

        virtual void render() = 0;

        virtual void serialize(nlohmann::json& document) {}

        virtual void deserialize(const nlohmann::json& document) {}

        [[nodiscard]] const std::string& getID() const { return this->id_; }

        explicit C_IWidget(const std::string_view& id) :
            id_(id), isVisible_(true) {}

        explicit C_IWidget(const std::string_view& id, const bool isVisible) :
            id_(id), isVisible_(isVisible) {}

        virtual ~C_IWidget() = default;
    };

    using widget_ptr_t = std::shared_ptr<C_IWidget>;

    using weak_widget_ptr_t = std::weak_ptr<C_IWidget>;
}

#endif //GUI_WIDGET_INTERFACE_H