//
// Created by sexey on 10.12.2025.
//

#ifndef DXX_DLC_SDK_USER_CMD_H
#define DXX_DLC_SDK_USER_CMD_H

#include "sdk/protobuf/gen/dota_usercmd.pb.h"

namespace sdk::datatype
{
    struct input_state_s
    {
        void* vtable;
        uint64_t value;
        uint64_t previousValue;
        uint64_t scrollValue;
    };

    struct user_cmd_s
    {
        void* vtable;
        int32_t sequenceNumber;
        dota::CDota2UserCmdPB cmd;
        input_state_s buttonState; //prob. unused
    };
}

#endif //DXX_DLC_SDK_USER_CMD_H