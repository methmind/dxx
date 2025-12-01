//
// Created by sexey on 11.11.2025.
//

#ifndef DXX_DLC_SDK_DOTA_VIEW_RENDER_H
#define DXX_DLC_SDK_DOTA_VIEW_RENDER_H

#include <windows.h>

namespace sdk::singleton
{
    constexpr auto ON_RENDER_START_VMT_INDEX = 4;

    class C_DotaViewRender
    {
    private:
        using get_view_render_instance_t = C_DotaViewRender*(__fastcall*)();

        void* instance_;

        bool findInstance(HMODULE clientModule);

    public:

        [[nodiscard]] FARPROC onRenderStart() const;

        bool initialize();

        C_DotaViewRender() = default;

        ~C_DotaViewRender() = default;
    };
} // sdk

#endif //DXX_DLC_SDK_DOTA_VIEW_RENDER_H