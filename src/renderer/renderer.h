//
// Created by sexey on 08.11.2025.
//

#ifndef DXX_DLC_RENDERER_H
#define DXX_DLC_RENDERER_H

#include <d3d11.h>
#include <dxgi.h>
#include <memory>

#include "gui/widget/gui_widget_button.h"

namespace render
{
    class C_Renderer
    {
    private:
        std::shared_ptr<gui::C_IWidget> guiRoot_;
        bool contextInited_;

        HWND targetWindowHandle_;
        ID3D11DeviceContext* d3dContext_;
        ID3D11RenderTargetView* renderTargetView_;

        static LRESULT CALLBACK hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

        bool initializeImGui(ID3D11Device* device) const;

        bool initializeDX11(IDXGISwapChain* swapChain);

        void dxgiPresent(IDXGISwapChain* self, UINT sync_interval, UINT flags);

    public:

        bool initialize();

        explicit C_Renderer(const std::shared_ptr<gui::C_IWidget>& root) :
            guiRoot_(root), contextInited_(false), targetWindowHandle_(nullptr),
            d3dContext_(nullptr), renderTargetView_(nullptr) {}

        ~C_Renderer();
    };
} // render

#endif //DXX_DLC_RENDERER_H