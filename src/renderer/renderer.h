//
// Created by sexey on 08.11.2025.
//

#ifndef DXX_DLC_RENDERER_H
#define DXX_DLC_RENDERER_H

#include <atomic>
#include <functional>
#include <d3d11.h>
#include <memory>

#include "imgui.h"
#include "renderer_list.h"

namespace render
{
    using on_render_callback = std::function<void()>;

    class C_Renderer
    {
    private:
        on_render_callback onRender_;
        bool contextInited_;

        HWND targetWindowHandle_;
        ID3D11DeviceContext* d3dContext_;
        ID3D11RenderTargetView* renderTargetView_;

        std::shared_ptr<C_RendererFrame> primitivesRenderFrame_;

        static LRESULT CALLBACK hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

        bool initializeImGui(ID3D11Device* device) const;

        bool initializeDX11(IDXGISwapChain* swapChain);

        void dxgiPresent(IDXGISwapChain* self, UINT sync_interval, UINT flags);

    public:

        std::shared_ptr<C_RendererFrame> getPrimitivesRenderFrame() { return this->primitivesRenderFrame_; }

        bool initialize(const on_render_callback& cb);

        explicit C_Renderer() :
            contextInited_(false), targetWindowHandle_(nullptr),
            d3dContext_(nullptr), renderTargetView_(nullptr),
            primitivesRenderFrame_(std::make_shared<C_RendererFrame>()) {}

        ~C_Renderer();
    };
} // render

#endif //DXX_DLC_RENDERER_H