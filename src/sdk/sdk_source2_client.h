//
// Created by sexey on 12.11.2025.
//

#ifndef DXX_DLC_SDK_SOURCE2_CLIENT_H
#define DXX_DLC_SDK_SOURCE2_CLIENT_H

#include <windows.h>

namespace sdk
{
    constexpr auto FRAME_STAGE_NOTIFY_VMT_INDEX = 36;

    class C_Source2Client
    {
    private:
        void* instance_;

    public:

        [[nodiscard]] FARPROC getFrameStageNotify() const;

        bool initialize();

        C_Source2Client() : instance_(nullptr) {}

        ~C_Source2Client() = default;
    };
} // sdk

#endif //DXX_DLC_SDK_SOURCE2_CLIENT_H