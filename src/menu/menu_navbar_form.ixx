//
// Created by sexey on 17.02.2026.
//
module;

export module menu.navbar;

import menu.navbar.designer;

namespace menu
{
    export constexpr auto MENU_NAVBAR_ID = "menu_navbar";

    export class C_MenuNavbarForm final : public C_MenuNavbarFormDesigner
    {
    public:
        ~C_MenuNavbarForm() override = default;

        C_MenuNavbarForm() : C_MenuNavbarFormDesigner(MENU_NAVBAR_ID) {}
    };
}