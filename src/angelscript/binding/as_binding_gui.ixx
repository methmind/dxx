//
// Created by sexey on 28.02.2026.
//
module;
#include <memory>

export module as.binding.gui;

import as.binding;
import as.engine_interface;

namespace as
{
    constexpr auto AS_GUI_NAMESPACE_NAME = "gui";

    export class C_ASBindingGUI : public C_IASBinding
    {
    public:
        [[nodiscard]] bool apply(const std::weak_ptr<C_IASEngine>& engineWeak) override
        {
            const auto enginePtr = engineWeak.lock();
            if (!enginePtr) {
                return false;
            }

            const auto engine = enginePtr->getEngine();
            engine->SetDefaultNamespace(AS_GUI_NAMESPACE_NAME);

            engine->SetDefaultNamespace("");
            return true;
        }
    };
}
