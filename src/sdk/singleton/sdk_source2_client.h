//
// Created by sexey on 12.11.2025.
//

#ifndef DXX_DLC_SDK_SOURCE2_CLIENT_H
#define DXX_DLC_SDK_SOURCE2_CLIENT_H

#include <windows.h>

namespace sdk::singleton
{
    /*
     * 48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B F1 33 FF 48 8B 89
     * void(__fastcall*)(void* instance, ClientFrameStage_t);
     */
    constexpr auto FRAME_STAGE_NOTIFY_VMT_INDEX = 36;

    /*
     * 48 8B 05 ? ? ? ? 48 8B 80 ? ? ? ? C3
     * void*(__fastcall*)(void* instance);
     */
    constexpr auto GET_NETWORK_CALLBACK_QUEUE = 34;

    class C_Source2Client
    {
    private:
        void* instance_;

    public:

        [[nodiscard]] FARPROC getFrameStageNotify() const;

        [[nodiscard]] FARPROC getGetNetworkCallbackQueue() const;

        bool initialize();

        C_Source2Client() : instance_(nullptr) {}

        ~C_Source2Client() = default;
    };
} // sdk

#endif //DXX_DLC_SDK_SOURCE2_CLIENT_H