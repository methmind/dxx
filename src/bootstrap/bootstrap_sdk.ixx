//
// Created by sexey on 14.02.2026.
//
module;
#include <memory>

export module bootstrap.sdk;

import service.container;

namespace bootstrap
{
    export bool InitializeSDK(std::unique_ptr<C_ServiceContainer>& services)
    {
        return true;
    }
}
