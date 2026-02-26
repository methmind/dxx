//
// Created by sexey on 17.02.2026.
//
module;
#include <memory>

export module as.binding;

import as.engine_interface;

namespace as
{
    export class C_IASBinding
    {
    public:
        virtual ~C_IASBinding() = default;

        [[nodiscard]] virtual bool apply(const std::weak_ptr<C_IASEngine>& enginePtr) = 0;
    };
}