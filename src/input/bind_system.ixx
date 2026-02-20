//
// Created by sexey on 20.02.2026.
//
module;
#include <array>
#include <cstdint>
#include <functional>
#include <optional>
#include <ranges>
#include <unordered_map>
#include <vector>
#include <windows.h>

#include "cs_shared_guarded.h"
#include "debug/debug_output.h"

export module bind_system;

import service.locator;
import hook.dispatcher;
import hook.type;

namespace input
{
    using callback_t = std::function<void()>;
    using bind_subscription_t = std::unique_ptr<void, std::function<void(void*)>>;

    struct callback_entry_s
    {
        uint64_t id;
        callback_t callback;
    };

    struct translated_event_s
    {
        int32_t virtualKey{};
        bool    isDown{};
        bool    isRepeat{};
    };

    export class C_BindSystem
    {
    public:
        C_BindSystem()
            : nextCallbackID_(0)
        {
            this->onMessageSubscription_ = C_ServiceLocator::Get<hook::C_HookDispatcher>()->subscribe<UINT, WPARAM, LPARAM>(
                static_cast<hook::hook_id_t>(hook::hook_type_e::WINDOW_MESSAGE),
                [this](UINT uMsg, WPARAM wParam, LPARAM lParam){
                    onWindowMessage(uMsg, wParam, lParam);
                }
            );
        }

        ~C_BindSystem() = default;

        C_BindSystem(const C_BindSystem&) = delete;
        C_BindSystem& operator=(const C_BindSystem&) = delete;

        [[nodiscard]] bind_subscription_t bind(int32_t virtualKey, callback_t callback)
        {
            const uint64_t id = this->nextCallbackID_.fetch_add(1, std::memory_order_relaxed);
            this->subscribers_.lock()->operator[](static_cast<UINT>(virtualKey)).push_back({ id, std::move(callback) });

            return bind_subscription_t(
                reinterpret_cast<void*>(id),
                [this](void* ptr) {
                    unbind(reinterpret_cast<uint64_t>(ptr));
                }
            );
        }

        void unbind(uint64_t subscriptionID)
        {
            for (const auto map = this->subscribers_.lock(); auto& entries: *map | std::views::values)
            {
                auto it = std::remove_if(entries.begin(), entries.end(),
                    [subscriptionID](const callback_entry_s& e) {
                        return e.id == subscriptionID;
                    }
                );

                entries.erase(it, entries.end());
            }
        }

        [[nodiscard]] bool isKeyDown(int32_t virtualKey) const
        {
            if (virtualKey < 0 || virtualKey >= 256) {
                return false;
            }

            return this->keyStates_.lock_shared()->at(static_cast<size_t>(virtualKey));
        }

    private:
        [[nodiscard]] static int32_t ResolveModifierKey(int32_t vk, UINT scanCode, bool isExtended)
        {
            switch (vk)
            {
                case VK_SHIFT:   return static_cast<int32_t>(MapVirtualKey(scanCode, MAPVK_VSC_TO_VK_EX));
                case VK_CONTROL: return isExtended ? VK_RCONTROL : VK_LCONTROL;
                case VK_MENU:    return isExtended ? VK_RMENU    : VK_LMENU;
                default:         return vk;
            }
        }

        [[nodiscard]] static int32_t RemapToUsLayout(int32_t vk, UINT scanCode)
        {
            static const HKL usLayout = LoadKeyboardLayoutW(L"00000409", KLF_NOTELLSHELL);
            if (const auto mapped = MapVirtualKeyExW(scanCode, MAPVK_VSC_TO_VK_EX, usLayout); mapped != 0) {
                return static_cast<int32_t>(mapped);
            }

            return vk;
        }

        [[nodiscard]] static std::optional<translated_event_s> TranslateEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            translated_event_s ev{};
            switch (uMsg)
            {
                case WM_KEYDOWN:
                case WM_SYSKEYDOWN:
                case WM_KEYUP:
                case WM_SYSKEYUP:
                {
                    const UINT scanCode = (static_cast<UINT>(lParam) >> 16) & 0xFF;
                    const bool isExtended = (lParam & (1 << 24)) != 0;
                    int32_t vk = static_cast<int32_t>(wParam);
                    if (vk == VK_SHIFT || vk == VK_CONTROL || vk == VK_MENU) {
                        vk = ResolveModifierKey(vk, scanCode, isExtended);
                    }
                    else {
                        vk = RemapToUsLayout(vk, scanCode);
                    }

                    ev.virtualKey = vk;
                    ev.isDown = (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN);
                    ev.isRepeat = ev.isDown && ((lParam & (1 << 30)) != 0);
                    break;
                }
                case WM_LBUTTONDOWN: case WM_LBUTTONUP:
                    ev.virtualKey = VK_LBUTTON;
                    ev.isDown     = (uMsg == WM_LBUTTONDOWN);
                    break;
                case WM_RBUTTONDOWN: case WM_RBUTTONUP:
                    ev.virtualKey = VK_RBUTTON;
                    ev.isDown = (uMsg == WM_RBUTTONDOWN);
                    break;
                case WM_MBUTTONDOWN: case WM_MBUTTONUP:
                    ev.virtualKey = VK_MBUTTON;
                    ev.isDown = (uMsg == WM_MBUTTONDOWN);
                    break;
                case WM_XBUTTONDOWN: case WM_XBUTTONUP:
                    ev.virtualKey = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? VK_XBUTTON1 : VK_XBUTTON2;
                    ev.isDown = (uMsg == WM_XBUTTONDOWN);
                    break;

                default:
                    return std::nullopt;
            }

            return ev;
        }

        void onWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            const auto ev = TranslateEvent(uMsg, wParam, lParam);
            if (!ev) {
                return;
            }

            if (ev->virtualKey >= 0 && ev->virtualKey < 256) {
                this->keyStates_.lock()->at(static_cast<size_t>(ev->virtualKey)) = ev->isDown;
            }

            if (!ev->isDown || ev->isRepeat) {
                return;
            }

            const auto map = this->subscribers_.lock_shared();
            const auto it  = map->find(static_cast<UINT>(ev->virtualKey));
            if (it == map->end()) {
                return;
            }

            for (const auto& [id, callback] : it->second) {
                callback();
            }
        }

        std::atomic<uint64_t> nextCallbackID_;

        libguarded::shared_guarded<std::array<bool, 256>> keyStates_;
        libguarded::shared_guarded<std::unordered_map<UINT, std::vector<callback_entry_s>>> subscribers_;

        hook::hook_subscription_t onMessageSubscription_;
    };

} // namespace input

