//
// Created by sexey on 07.11.2025.
//

#ifndef DXX_DLC_BOOTSTRAP_GUI_H
#define DXX_DLC_BOOTSTRAP_GUI_H

#include "service_locator/service_container.h"

namespace bootstrap
{
    bool InitializeGuiStuff(const std::shared_ptr<C_ServiceContainer>& services);
} // bootstrap

#endif //DXX_DLC_BOOTSTRAP_GUI_H