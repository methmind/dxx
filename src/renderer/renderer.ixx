//
// Created by sexey on 15.02.2026.
//
module;
#include <cstdint>
#include <d3d11.h>
#include <filesystem>
#include <functional>
#include <memory>
#include "shlobj.h"

#include "imgui_internal.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "debug/debug_output.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

export module renderer;

import service.locator;
import hook.type;
import hook.dispatcher;

namespace render
{
    struct com_destructor_s
    {
        void operator()(IUnknown* ptr) const
        {
            if (ptr) {
                ptr->Release();
            }
        }
    };

    template<typename T>
    using com_ptr_t = std::unique_ptr<T, com_destructor_s>;

    template<typename T>
    [[nodiscard]] com_ptr_t<T> make_com_ptr(T* raw) { return com_ptr_t<T>(raw); }

    static WNDPROC originalWndProc = nullptr;

    export class C_Renderer
    {
    public:
        ~C_Renderer()
        {
            this->onPresentSubscription_.reset();

            if (this->targetWnd_ && originalWndProc) {
                SetWindowLongPtr(this->targetWnd_, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(originalWndProc));
                originalWndProc = nullptr;
            }

            if (this->isContextInitialized_.load(std::memory_order_relaxed)) {
                ImGui_ImplDX11_Shutdown();
                ImGui_ImplWin32_Shutdown();
                ImGui::DestroyContext();
            }
        }

        C_Renderer() : isContextInitialized_(false), targetWnd_(nullptr)
        {
            const auto dispatcher = C_ServiceLocator::Get<hook::C_HookDispatcher>();
            this->onPresentSubscription_ = dispatcher->subscribe<IDXGISwapChain*, UINT, UINT>(
                static_cast<uint16_t>(hook::hook_type_e::PRESENT),
                [this](IDXGISwapChain* swapChain, UINT syncInterval, UINT flags) {
                    onPresent(swapChain, syncInterval, flags);
                }
            );
        }

        [[nodiscard]] bool isContextInitialized() const { return this->isContextInitialized_.load(std::memory_order_acquire); }

        [[nodiscard]] ID3D11Device* getDevice() const { return this->d3dDevice_.get(); }

    private:
        static LRESULT hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            C_ServiceLocator::Get<hook::C_HookDispatcher>()->invoke<static_cast<uint16_t>(hook::hook_type_e::WINDOW_MESSAGE)>(
                uMsg, wParam, lParam
            );

            if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam) > 0) {
                return 1L;
            }

            if (const auto& io = ImGui::GetIO(); io.WantCaptureMouse || io.WantCaptureKeyboard) {
                return 1L;
            }

            return CallWindowProc(originalWndProc, hWnd, uMsg, wParam, lParam);
        }

        [[nodiscard]] bool initializeImGui(ID3D11Device* device) const
        {
            PWSTR fontsRootPath = nullptr;
            if (const auto err = SHGetKnownFolderPath(FOLDERID_Fonts, 0, nullptr, &fontsRootPath); err != S_OK) {
                dbg("SHGetKnownFolderPath got:err = {}", err);
                return false;
            }

            const auto fontPath = (std::filesystem::path(fontsRootPath) / "Verdana.ttf").string();
            CoTaskMemFree(fontsRootPath);

            ImGui::CreateContext();

            auto& io = ImGui::GetIO();
            io.IniFilename = nullptr;
            io.LogFilename = nullptr;
            io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

            io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 15.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());

            ImGui_ImplWin32_Init(this->targetWnd_);
            ImGui_ImplDX11_Init(device, this->d3dContext_.get());

            return true;
        }

        void releaseRenderTarget() { this->renderTargetView_.reset(); }

        [[nodiscard]] bool createRenderTarget(IDXGISwapChain* swapChain)
        {
            ID3D11Texture2D* rawBackBuffer = nullptr;
            if (const auto err = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&rawBackBuffer)); err != S_OK) {
                dbg("IDXGISwapChain::GetBuffer got:err = {}", err);
                return false;
            }

            auto backBuffer = make_com_ptr(rawBackBuffer);
            ID3D11RenderTargetView* rawRtv = nullptr;
            if (const auto err = this->d3dDevice_->CreateRenderTargetView(backBuffer.get(), nullptr, &rawRtv); err != S_OK) {
                dbg("ID3D11Device::CreateRenderTargetView got:err = {}", err);
                return false;
            }

            this->renderTargetView_ = make_com_ptr(rawRtv);
            return true;
        }

        [[nodiscard]] bool initializeDX11(IDXGISwapChain* swapChain)
        {
            if (this->isContextInitialized_.load(std::memory_order_relaxed)) {
                return true;
            }

            DXGI_SWAP_CHAIN_DESC description;
            if (const auto err = swapChain->GetDesc(&description); err != S_OK) {
                dbg("IDXGISwapChain::GetDesc got:err = {}", err);
                return false;
            }

            ID3D11Device* rawDevice = nullptr;
            if (const auto err = swapChain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&rawDevice)); err != S_OK) {
                dbg("IDXGISwapChain::GetDevice got:err = {}", err);
                return false;
            }

            this->d3dDevice_ = make_com_ptr(rawDevice);

            ID3D11DeviceContext* rawContext = nullptr;
            this->d3dDevice_->GetImmediateContext(&rawContext);
            this->d3dContext_ = make_com_ptr(rawContext);

            if (!createRenderTarget(swapChain)) {
                dbg("createRenderTarget got:err = Unable to create render target!");
                return false;
            }

            this->targetWnd_ = description.OutputWindow;
            originalWndProc = reinterpret_cast<WNDPROC>(
                SetWindowLongPtr(this->targetWnd_, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hkWndProc))
            );

            if (!initializeImGui(this->d3dDevice_.get())) {
                dbg("initializeImGui got:err = Unable to initialize ImGui!");
                return false;
            }

            this->isContextInitialized_.store(true, std::memory_order_release);
            return true;
        }

        void onPresent(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags)
        {
            if (!initializeDX11(swapChain)) {
                return;
            }

            if (!this->renderTargetView_ && !createRenderTarget(swapChain)) {
                return;
            }

            const auto dispatcher = C_ServiceLocator::Get<hook::C_HookDispatcher>();
            dispatcher->invoke<static_cast<uint16_t>(hook::hook_type_e::ON_PRE_IMGUI_RENDER)>();

            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();

            dispatcher->invoke<static_cast<uint16_t>(hook::hook_type_e::ON_IMGUI_RENDER)>(
                ImGui::GetBackgroundDrawList()
            );

            ImGui::EndFrame();
            ImGui::Render();

            auto* rtv = this->renderTargetView_.get();
            this->d3dContext_->OMSetRenderTargets(1, &rtv, nullptr);
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        }

        std::atomic<bool> isContextInitialized_;
        HWND targetWnd_;

        com_ptr_t<ID3D11Device> d3dDevice_;
        com_ptr_t<ID3D11DeviceContext> d3dContext_;
        com_ptr_t<ID3D11RenderTargetView> renderTargetView_;

        hook::hook_subscription_t onPresentSubscription_;
    };
}



