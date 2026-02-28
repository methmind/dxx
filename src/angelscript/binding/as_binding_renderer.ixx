//
// Created by sexey on 23.02.2026.
//
module;
#include <memory>
#include <windows.h>

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"

#include "asbind20/bind/class.hpp"
#include "asbind20/bind/global.hpp"

export module as.binding.renderer;

import service.locator;

import renderer;
import renderer.load_image;
import worker_queue;

import as.binding;
import as.engine_interface;
import as.binding.memory;

import sdk.matrices_system;
import sdk.math.vector;
import sdk.math.world_to_screen;
import sdk.source2_engine_to_client;

namespace as
{
    constexpr auto AS_RENDERER_NAMESPACE_NAME = "render";

    using d3d_texture_wrapper_t = C_SharedPtr<render::d3d_texture_t>;

    export class C_ASBindingRenderer : public C_IASBinding
    {
    public:
        C_ASBindingRenderer(const std::shared_ptr<render::C_Renderer>& renderer,
            const std::shared_ptr<C_WorkerQueue>& preRenderQueue,
            const std::shared_ptr<sdk::C_MatricesSystem>& matricesSystem
        ) : renderer_(renderer), preRenderQueue_(preRenderQueue), matricesSystem_(matricesSystem) {}

        [[nodiscard]] bool apply(const std::weak_ptr<C_IASEngine>& engineWeak) override
        {
            const auto enginePtr = engineWeak.lock();
            if (!enginePtr) {
                return false;
            }

            const auto engine = enginePtr->getEngine();
            engine->SetDefaultNamespace(AS_RENDERER_NAMESPACE_NAME);

            asbind20::ref_class<d3d_texture_wrapper_t>(engine, "d3d_texture_t")
                .addref(&d3d_texture_wrapper_t::addRef)
                .release(&d3d_texture_wrapper_t::release);

            asbind20::ref_class<ImDrawList>(engine, "C_Frame", asOBJ_NOCOUNT)
                .method("void addLine(imgui::ImVec2, imgui::ImVec2, uint, float)",
                [](ImDrawList& self, ImVec2 start, ImVec2 end, uint32_t color, float thickness) {
                    self.AddLine(start, end, color, thickness);
                })
                .method("void addText(const string& in, imgui::ImVec2, uint)",
                [](ImDrawList& self, const std::string& text, ImVec2 pos, uint32_t color) {
                    self.AddText(pos, color, text.c_str());
                })
                .method("void addFontText(const string& in, imgui::ImFont@, float fontSize, imgui::ImVec2, uint)",
                [](ImDrawList& self, const std::string& text, ImFont* font, float fontSize, ImVec2 pos, uint32_t color) {
                    if (!font) {
                        asbind20::set_script_exception("Font couldnt be a nullptr!");
                        return;
                    }

                    if (!font->IsLoaded()) {
                        font = ImGui::GetDefaultFont();
                    }

                    self.AddText(font, fontSize, pos, color, text.c_str());
                })
                .method("void addRect(imgui::ImVec4, uint, float, float)",
                [](ImDrawList& self, ImVec4 pos, uint32_t color, float rounding, float thickness) {
                    self.AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), color, rounding, 0, thickness);
                })
                .method("void addFilledRect(imgui::ImVec4, uint, float)",
                [](ImDrawList& self, ImVec4 pos, uint32_t color, float rounding) {
                    self.AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), color, rounding, 0);
                })
                .method("void addTriangle(imgui::ImVec2, imgui::ImVec2, imgui::ImVec2, uint, float)",
                [](ImDrawList& self, ImVec2 a, ImVec2 b, ImVec2 c, uint32_t color, float thickness) {
                    self.AddTriangle(a, b, c, color, thickness);
                })
                .method("void addFilledTriangle(imgui::ImVec2, imgui::ImVec2, imgui::ImVec2, uint)",
                [](ImDrawList& self, ImVec2 a, ImVec2 b, ImVec2 c, uint32_t color) {
                    self.AddTriangleFilled(a, b, c, color);
                })
                .method("void addCircle(imgui::ImVec2, float, uint, int, float)",
                [](ImDrawList& self, ImVec2 pos, float radius, uint32_t color, int32_t seg, float thickness) {
                    self.AddCircle(pos, radius, color, seg, thickness);
                })
                .method("void addFilledCircle(imgui::ImVec2, float, uint, int)",
                [](ImDrawList& self, ImVec2 pos, float radius, uint32_t color, int32_t seg) {
                    self.AddCircleFilled(pos, radius, color, seg);
                })
                .method("void addImage(render::d3d_texture_t@+, imgui::ImVec2, imgui::ImVec2, uint)",
                [](ImDrawList& self, const d3d_texture_wrapper_t* texture, ImVec2 pos, ImVec2 size, uint32_t color) {
                    if (!texture || !texture->get() || !texture->get()->get()) {
                        asbind20::set_script_exception("Texture couldnt be a nullptr!");
                        return;
                    }

                    const ImVec2 p_max(pos.x + size.x, pos.y + size.y);
                    self.AddImage(texture->get()->get(), pos, p_max,
                        ImVec2(0, 0), ImVec2(1, 1), color
                    );
                });

            asbind20::global(engine)
                .function("uint64 getTickCount()", [] {
                    return GetTickCount64();
                })
                .function("imgui::ImVec2 measureText(const string& in)",
                [](const std::string& text) {
                    return ImGui::CalcTextSize(text.c_str());
                })
                .function("render::d3d_texture_t@ loadVTEX(const string& in)",
                    &C_ASBindingRenderer::loadVTEX, asbind20::auxiliary(this)
                )
                .function("render::d3d_texture_t@ loadPNG(const string& in)",
                    &C_ASBindingRenderer::loadPNG, asbind20::auxiliary(this)
                )
                .function("imgui::ImFont@ loadFont(const string& in, float)",
                    &C_ASBindingRenderer::loadFont, asbind20::auxiliary(this)
                ).function("imgui::ImVec2 worldToScreen(float, float, float)",
                    &C_ASBindingRenderer::worldToScreen, asbind20::auxiliary(this)
                );

            engine->SetDefaultNamespace("");
            return true;
        }

    private:
        ImFont* loadFont(const std::string& path, float size) const
        {
            const auto& io = ImGui::GetIO();
            const auto newFont = io.Fonts->AddFontFromFileTTF(path.c_str(), size, nullptr, io.Fonts->GetGlyphRangesCyrillic());
            if (newFont) {
                this->preRenderQueue_->enqueue([] {
                    ImGui_ImplDX11_InvalidateDeviceObjects();
                    ImGui_ImplDX11_CreateDeviceObjects();
                });
            }

            return newFont;
        }

        d3d_texture_wrapper_t* loadVTEX(const std::string& path) const
        {
            auto imageData = render::LoadVTEX(this->renderer_->getDevice(), path);
            if (!imageData) {
                asbind20::set_script_exception("Failed to load image: " + path);
                return nullptr;
            }

            return new d3d_texture_wrapper_t(std::make_shared<render::d3d_texture_t>(std::move(imageData)));
        }

        d3d_texture_wrapper_t* loadPNG(const std::string& path) const
        {
            auto imageData = render::LoadPNG(this->renderer_->getDevice(), path);
            if (!imageData) {
                asbind20::set_script_exception("Failed to load image: " + path);
                return nullptr;
            }

            return new d3d_texture_wrapper_t(std::make_shared<render::d3d_texture_t>(std::move(imageData)));
        }

        ImVec2 worldToScreen(float x, float y, float z) const
        {
            sdk::vector2_t output;
            if (!sdk::WorldToScreen(sdk::vector3_t(x, y, z),
                C_ServiceLocator::Get<sdk::C_Source2EngineToClient>()->getScreenSize(),
                this->matricesSystem_->getWorldToProjection(), output)) {
                return ImVec2(-1, -1);
            }

            return ImVec2(output.x, output.y);
        }

        std::shared_ptr<render::C_Renderer> renderer_;
        std::shared_ptr<C_WorkerQueue> preRenderQueue_;
        std::shared_ptr<sdk::C_MatricesSystem> matricesSystem_;
    };
}
