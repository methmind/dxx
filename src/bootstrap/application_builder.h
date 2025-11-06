//
// Created by sexey on 29.10.2025.
//

#ifndef APPLICATION_BUILDER_H
#define APPLICATION_BUILDER_H

#include "service_locator/service_container.h"

namespace bootstrap
{
    class C_ApplicationBuilder
    {
    public:
        static std::shared_ptr<C_ServiceContainer> Build();
    };
} // bootstrap

#endif //APPLICATION_BUILDER_H