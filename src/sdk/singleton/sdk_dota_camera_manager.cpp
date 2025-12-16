//
// Created by sexey on 15.12.2025.
//

#include "sdk_dota_camera_manager.h"
#include <windows.h>

#include "debug/debug_output.h"
#include "memory/pattern_scanner.h"
#include "memory/vmt_call.h"
#include "sdk/sdk_signature.h"

namespace sdk::singleton
{
    datatype::C_DotaCamera* C_DotaCamaraManager::getPrimaryCamera() const
    {
        return ::memory::vmt::call<datatype::C_DotaCamera*(__fastcall*)(void* instance), GET_CAMERA_INSTANCE_VMT_INDEX>(
            this->instance_
        );
    }

    bool C_DotaCamaraManager::initialize()
    {
        const auto getInstance = reinterpret_cast<FARPROC>(memory::FindPattern(GetModuleHandleA("client.dll"), signature::GET_DOTA_CAMERA_MANAGER_FUNC));
        if (!getInstance) {
            dbg("Unable to find C_DotaCamaraManager::GetInstance function!");
            return false;
        }

        this->instance_ = reinterpret_cast<void*>(getInstance());
        if (!this->instance_) {
            dbg("C_DotaCamaraManager::GetInstance returned nullptr!");
            return false;
        }

        dbg("C_DotaCamaraManager ptr: %p", this->instance_);
        return true;
    }
} // sdk