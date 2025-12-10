//
// Created by sexey on 10.12.2025.
//

#ifndef DXX_DLC_SDK_USER_CMD_H
#define DXX_DLC_SDK_USER_CMD_H

#include "sdk/protobuf/gen/dota_usercmd.pb.h"

namespace sdk::datatype
{
    struct user_cmd_s
    {
        void* vtable;
        int32_t sequenceNumber;
        dota::CDota2UserCmdPB cmd;
        dota::CMsgVector* crosshair;
    };
}

#endif //DXX_DLC_SDK_USER_CMD_H