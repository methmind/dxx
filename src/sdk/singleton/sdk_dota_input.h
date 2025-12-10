//
// Created by sexey on 10.12.2025.
//

#ifndef DXX_DLC_SDK_DOTA_INPUT_H
#define DXX_DLC_SDK_DOTA_INPUT_H

#include <windows.h>

#include "sdk/datatype/sdk_dota_player_controller.h"
#include "sdk/datatype/sdk_user_cmd.h"

namespace sdk::singleton
{
    class C_DotaInput
    {
    public:
        using create_move_t = void(__fastcall*)(void* cinput, int32_t slot, bool isActive);
        using get_screen_cmd_buffer_t = void*(__fastcall*)(void* cmdCircularBuffer, int32_t screenID);
        using get_user_cmd_t = void*(__fastcall*)(void* playerController, int32_t sequenceNumber);
        using get_screen_player_controller_t = datatype::C_DotaPlayerController*(__fastcall*)(int32_t splitScreenID);

    private:
        create_move_t createMove_;
        get_screen_cmd_buffer_t getScreenCmdBuffer_;
        get_user_cmd_t getUserCmd_;
        get_screen_player_controller_t getScreenPlayerController_;

        void* cmdCircularBuffer_;
        uint32_t sequenceNumberOffset_;

        bool resolveFunctions(HMODULE clientModule);

        bool resolveGlobalCmdBuffer(HMODULE clientModule);

        bool resolveSequenceNumberOffset(HMODULE clientModule);

    public:

        [[nodiscard]] datatype::user_cmd_s* getUserCmd() const;

        [[nodiscard]] create_move_t getCreateMove() const { return this->createMove_; }

        bool initialize();

        C_DotaInput() = default;

        ~C_DotaInput() = default;
    };
} // sdk

#endif //DXX_DLC_SDK_DOTA_INPUT_H