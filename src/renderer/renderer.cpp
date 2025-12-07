//
// Created by sexey on 08.11.2025.
//

#include "renderer.h"

#include <d3d11.h>
#include <filesystem>

#include "shlobj.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "debug/debug_output.h"
#include "hook/hook_dispatcher.h"
#include "hook/impl/hook_impl_present.h"
#include "hook/impl/hook_impl_type.h"
#include "input/bind_system.h"
#include "service_locator/service_locator.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace render
{
    static WNDPROC oWndProc = nullptr;

    LRESULT C_Renderer::hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN) {
            C_ServiceLocator::getInstance<input::C_BindSystem>()->invokeBindings(static_cast<uint8_t>(wParam));
        }

        if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam) > 0) {
            return 1L;
        }

        return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
    }

    bool C_Renderer::initializeImGui(ID3D11Device* device) const
    {
        PWSTR fontsRootPath;
        if (const auto err = SHGetKnownFolderPath(FOLDERID_Fonts, 0, nullptr, &fontsRootPath);
            err != S_OK) {
            dbg("SHGetKnownFolderPath got:err = %d", err);
            return false;
        }

        ImGui::CreateContext();

        auto& io = ImGui::GetIO();
        io.IniFilename = nullptr;
        io.LogFilename = nullptr;
        io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        io.Fonts->AddFontFromFileTTF(
            (std::filesystem::path(fontsRootPath) / "Verdana.ttf").string().c_str(), 15.0f
        );

        ImGui_ImplWin32_Init(this->targetWindowHandle_);
        ImGui_ImplDX11_Init(device, this->d3dContext_);

        CoTaskMemFree(fontsRootPath);
        return true;
    }

    bool C_Renderer::initializeDX11(IDXGISwapChain* swapChain)
    {
        if (this->contextInited_) {
            return true;
        }

        DXGI_SWAP_CHAIN_DESC desc;
        if (const auto err = swapChain->GetDesc(&desc); err != S_OK) {
            dbg("IDXGISwapChain::GetDesc got:err = %d", err);
            return false;
        }

        ID3D11Device* device;
        if (const auto err = swapChain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&device));
            err != S_OK) {
            dbg("IDXGISwapChain::GetDevice got:err = %d", err);
            return false;
        }

        device->GetImmediateContext(&this->d3dContext_);
        ID3D11Texture2D* backBuffer;
        if (const auto err = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
            reinterpret_cast<LPVOID*>(&backBuffer)); err != S_OK) {
            dbg("IDXGISwapChain::GetBuffer got:err = %d", err);
            return false;
        }

        if (const auto err = device->CreateRenderTargetView(backBuffer, nullptr,
            &this->renderTargetView_); err != S_OK) {
            dbg("ID3D11Device::CreateRenderTargetView got:err = %d", err);
            return false;
        }

        backBuffer->Release();
        this->targetWindowHandle_ = desc.OutputWindow;

        oWndProc = reinterpret_cast<WNDPROC>(
            SetWindowLongPtr(this->targetWindowHandle_, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hkWndProc))
        );

        if (!initializeImGui(device)) {
            dbg("Unable to initialize ImGui!");
            return false;
        }

        return (this->contextInited_ = true);
    }

    void C_Renderer::dxgiPresent(IDXGISwapChain* self, UINT sync_interval, UINT flags)
    {
        if (!initializeDX11(self)) {
            assert("Unable to initialize DX11 renderer!");
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        this->primitivesRenderFrame_->render(ImGui::GetBackgroundDrawList());
        this->onRender_();

        ImGui::EndFrame();
        ImGui::Render();

        this->d3dContext_->OMSetRenderTargets(1, &this->renderTargetView_, nullptr);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    bool C_Renderer::initialize(const on_render_callback& cb)
    {
        this->onRender_ = cb;
        C_ServiceLocator::getInstance<hook::C_HookDispatcher>()->subscribe<IDXGISwapChain*, UINT, UINT>(
            static_cast<hook::hook_id_t>(hook::impl::hook_impl_type_e::PRESENT),
            [this](IDXGISwapChain* self, UINT sync_interval, UINT flags) {
                dxgiPresent(self, sync_interval, flags);
            }
        );

        return true;
    }

    C_Renderer::~C_Renderer()
    {
        SetWindowLongPtr(this->targetWindowHandle_, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(oWndProc));
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }
} // render