//
// Created by sexey on 17.02.2026.
//
module;
#include <memory>
#include <string_view>

export module menu.navbar.designer;

import menu;
import gui.widget.navbar;
import gui.widget.context_menu;
import gui.widget.separator;
import gui.widget.windows_container;

namespace menu
{
    export class C_MenuNavbarFormDesigner : public C_IForm
    {
    public:
        ~C_MenuNavbarFormDesigner() override = default;

        explicit C_MenuNavbarFormDesigner(const std::string_view& id) :
            C_IForm(id), scriptsContainer_(nullptr), systemContainer_(nullptr) {}

        auto getSystemContainer() const { return this->systemContainer_; }

        auto getScriptContainer() const { return this->scriptsContainer_; }

        bool initialize() override
        {
            const auto navbar = std::make_shared<gui::C_WidgetNavbar>("navbar");
            const auto navbarContextMenu = std::make_shared<gui::C_WidgetContextMenu>("navbar_context_menu", "Dota++");

            const auto scriptSeparator = std::make_shared<gui::C_WidgetSeparator>("script_separator", "Scripts");
            this->scriptsContainer_ = std::make_shared<gui::C_WidgetWindowsContainer>("navbar_scripts_container");

            const auto systemSeparator = std::make_shared<gui::C_WidgetSeparator>("system_separator", "System");
            this->systemContainer_ = std::make_shared<gui::C_WidgetWindowsContainer>("navbar_system_container");

            navbarContextMenu->addChild(scriptSeparator);
            navbarContextMenu->addChild(this->scriptsContainer_);
            navbarContextMenu->addChild(systemSeparator);
            navbarContextMenu->addChild(this->systemContainer_);
            navbar->addChild(navbarContextMenu);
            addChild(navbar);

            return true;
        }

    protected:
        std::shared_ptr<gui::C_WidgetWindowsContainer> scriptsContainer_;
        std::shared_ptr<gui::C_WidgetWindowsContainer> systemContainer_;
    };
}