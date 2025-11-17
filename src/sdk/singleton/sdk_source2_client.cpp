//
// Created by sexey on 12.11.2025.
//

#include "sdk_source2_client.h"
#include <windows.h>

#include "debug/debug_output.h"
#include "sdk/interface/sdk_interface_scanner.h"

namespace sdk::singleton
{
    FARPROC C_Source2Client::getFrameStageNotify() const
    {
        const auto vtable = *static_cast<void***>(this->instance_);
        return reinterpret_cast<FARPROC>(vtable[FRAME_STAGE_NOTIFY_VMT_INDEX]);
    }

    bool C_Source2Client::initialize()
    {
        if (this->instance_ = iface::Find(GetModuleHandleA("client.dll"), "Source2Client0"); !this->instance_) {
            dbg("Unable to find Source2Client instance!");
            return false;
        }

        dbg("C_Source2Client ptr: %p", this->instance_);
        return true;
    }
} // sdk