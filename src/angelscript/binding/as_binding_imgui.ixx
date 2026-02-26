//
// Created by sexey on 22.02.2026.
//
module;
#include <algorithm>
#include <memory>

#include "imgui.h"
#include "asbind20/asbind.hpp"

export module as.binding.imgui;

import as.binding;
import as.engine_interface;

namespace as
{
    constexpr auto AS_IMGUI_NAMESPACE_NAME = "imgui";

    export class C_ASBindingImgui : public C_IASBinding
    {
    public:
        C_ASBindingImgui() = default;

        [[nodiscard]] bool apply(const std::weak_ptr<C_IASEngine>& engineWeak) override
        {
            const auto enginePtr = engineWeak.lock();
            if (!enginePtr) {
                return false;
            }

            const auto engine = enginePtr->getEngine();
            engine->SetDefaultNamespace(AS_IMGUI_NAMESPACE_NAME);

            asbind20::value_class<ImVec2>(engine, "ImVec2", asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS)
                .constructor<float, float>("float, float")
                .property("float x", &ImVec2::x)
                .property("float y", &ImVec2::y);

            asbind20::value_class<ImVec4>(engine, "ImVec4", asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS)
                .constructor<float, float, float, float>("float, float, float, float")
                .property("float x", &ImVec4::x)
                .property("float y", &ImVec4::y)
                .property("float z", &ImVec4::z)
                .property("float w", &ImVec4::w);

            asbind20::ref_class<ImFont>(engine, "ImFont", asOBJ_NOCOUNT)
                .method("float getFontSize()",
                [](const ImFont& self) {
                    return self.LastBaked->Size;
                });

            asbind20::global(engine).function("uint color32(uint8 r, uint8 g, uint8 b, uint8 a)", [] (uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
                return IM_COL32(r, g, b, a);
            });

            engine->SetDefaultNamespace("");
            return true;
        }
    };
}
