//
// Created by sexey on 01.12.2025.
//

#ifndef DXX_DLC_SDK_RENDER_GAME_SYSTEM_H
#define DXX_DLC_SDK_RENDER_GAME_SYSTEM_H

#include <windows.h>

namespace sdk::singleton
{
    constexpr auto RENDER_GAME_SYSTEM_SID = "RenderGameSystem";

    class C_RenderGameSystem
    {
    private:
        void* instance_;
        FARPROC getMatricesForView_;

    public:

        [[nodiscard]] FARPROC getMatricesForView() const { return this->getMatricesForView_; }

        bool initialize();

        C_RenderGameSystem() = default;

        ~C_RenderGameSystem() = default;
    };
} // sdk

#endif //DXX_DLC_SDK_RENDER_GAME_SYSTEM_H