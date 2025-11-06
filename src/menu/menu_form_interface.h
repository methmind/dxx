//
// Created by sexey on 29.10.2025.
//

#ifndef MENU_FORM_INTERFACE_H
#define MENU_FORM_INTERFACE_H

#include "gui/gui_container_interface.h"

namespace menu
{
    class C_IMenuForm : public gui::C_IContainer
    {
    public:

        void render() override {
            if (!isVisible()) {
                return;
            }

            for (auto& childVector = getChildList(); const auto& child : childVector) {
                child->render();
            }
        }

        virtual bool initialize() = 0;

        explicit C_IMenuForm(const std::string_view& id) : C_IContainer(id) {}
    };
}

#endif //MENU_FORM_INTERFACE_H