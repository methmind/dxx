//
// Created by sexey on 17.02.2026.
//
module;
#include <memory>
#include <string>

#include "nlohmann/json.hpp"

export module gui.widget.base;

import gui.widget.type;

export namespace gui
{
    class C_WidgetBase : public std::enable_shared_from_this<C_WidgetBase>
    {
    public:
        virtual ~C_WidgetBase() = default;

        explicit C_WidgetBase(const std::string_view& id) : id_(id), isVisible_(true) {}

        C_WidgetBase(const C_WidgetBase&) = delete;

        C_WidgetBase& operator=(const C_WidgetBase&) = delete;

        [[nodiscard]] const std::string& getID() const { return this->id_; }

        [[nodiscard]] virtual widget_flags_e getFlags() const { return widget_flags_e::DEFAULT; }

        virtual void setVisibleState(const bool isVisible) { this->isVisible_ = isVisible; }

        [[nodiscard]] bool getVisibleState() const { return this->isVisible_; }

        void setParent(const std::weak_ptr<C_WidgetBase>& parent) { this->parent_ = parent; }

        [[nodiscard]] std::weak_ptr<C_WidgetBase> getParent() const { return this->parent_; }

        virtual void serialize(nlohmann::json& document) {}

        virtual void deserialize(const nlohmann::json& document) {}

        virtual void render() = 0;
    private:
        std::string id_;
        bool isVisible_;
        std::weak_ptr<C_WidgetBase> parent_;
    };

    using widget_ptr_t = std::shared_ptr<C_WidgetBase>;

    using weak_widget_ptr_t = std::weak_ptr<C_WidgetBase>;
}