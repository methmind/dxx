//
// Created by sexey on 29.11.2025.
//

#include "lua_binding_renderer.h"

#include "debug/debug_output.h"

namespace lua::binding
{
    void C_LuaBindingRenderer::RegisterRendererPrimitivesFrame(sol::state& state)
    {
        //@note Dont use this variable outside of the on_render_start callback!!!
        auto luaRendererFrame = state.new_usertype<render::C_RendererFrame>(
            "C_RendererFrame", sol::no_constructor
        );

        luaRendererFrame.set_function("add_line",
            [](const render::C_RendererFrame& self, ImVec2 start, ImVec2 end, ImU32 color, float thickness) {
                self.enqueue([start, end, color, thickness](ImDrawList* ctx) {
                    ctx->AddLine(start, end, color, thickness);
                });
            }
        );

        luaRendererFrame.set_function("add_text",
            [](const render::C_RendererFrame& self, ImVec2 pos, const std::string& title, ImU32 color) {
                self.enqueue([pos, color, title](ImDrawList* ctx) {
                    ctx->AddText(pos, color, title.c_str());
                });
            }
        );

        luaRendererFrame.set_function("add_rect",
            [](const render::C_RendererFrame& self, ImVec4 pos, ImU32 color, float rounding, float thickness) {
                self.enqueue([pos, color, rounding, thickness](ImDrawList* ctx) {
                    ctx->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w),
                        color, rounding, 0, thickness);
                    }
                );
            }
        );

        luaRendererFrame.set_function("add_filled_rect",
            [](const render::C_RendererFrame& self, ImVec4 pos, ImU32 color, float rounding) {
                self.enqueue([pos, color, rounding](ImDrawList* ctx) {
                    ctx->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), color, rounding);
                }
            );
        });

        luaRendererFrame.set_function("add_triangle",
            [](const render::C_RendererFrame& self, ImVec2 a, ImVec2 b, ImVec2 c, ImU32 color, float thickness) {
                self.enqueue([a, b, c, color, thickness](ImDrawList* ctx) {
                    ctx->AddTriangle(a, b, c, color, thickness);
                });
            }
        );

        luaRendererFrame.set_function("add_filled_triangle",
            [](const render::C_RendererFrame& self, ImVec2 a, ImVec2 b, ImVec2 c, ImU32 color) {
                self.enqueue([a, b, c, color](ImDrawList* ctx) {
                    ctx->AddTriangleFilled(a, b, c, color);
                });
            }
        );

        luaRendererFrame.set_function("add_circle",
            [](const render::C_RendererFrame& self, ImVec2 pos, float radius, ImU32 color, int32_t seg, float thickness) {
                self.enqueue([pos, radius, color, seg, thickness](ImDrawList* ctx) {
                    ctx->AddCircle(pos, radius, color, seg, thickness);
                });
            }
        );

        luaRendererFrame.set_function("add_filled_circle",
            [](const render::C_RendererFrame& self, ImVec2 pos, float radius, ImU32 color, int32_t seg) {
                self.enqueue([pos, radius, color, seg](ImDrawList* ctx) {
                    ctx->AddCircleFilled(pos, radius, color, seg);
                });
            }
        );

        //@note Undocumented feature!!!
        luaRendererFrame.set_function("bake", &render::C_RendererFrame::bake);
    }

    bool C_LuaBindingRenderer::apply(const std::weak_ptr<C_ILuaGuardedState>& guardedState)
    {
        const auto tmp = guardedState.lock()->getLuaState();
        auto& luaState = *tmp;

        auto rendererNamespace = luaState[RENDERER_NAMESPACE_NAME].get_or_create<sol::table>();
        if (!rendererNamespace.valid()) {
            dbg("Unable to create renderer namespace!");
            return false;
        }

        RegisterRendererPrimitivesFrame(luaState);

        //@note Undocumented feature!!!
        rendererNamespace.set_function("get_renderer_primitives", [this] {
           return this->renderer_->getPrimitivesRenderFrame();
        });

        rendererNamespace.set_function("get_screen_size", [] { return ImGui::GetIO().DisplaySize; });
        rendererNamespace.set_function("measure_text", [](const char* text) { return ImGui::CalcTextSize(text); });

        return true;
    }
} // lua