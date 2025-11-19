//
// Created by sexey on 19.11.2025.
//

#include "sdk_schema_system.h"

#include "debug/debug_output.h"
#include "sdk/interface/sdk_interface_scanner.h"

namespace sdk::singleton
{
    bool C_ShemaSystem::initialize()
    {
        if (this->instance_ = iface::Find(GetModuleHandleA("schemasystem.dll"), "SchemaSystem_"); !this->instance_) {
            dbg("Unable to find ShemaSystem instance!");
            return false;
        }

        dbg("C_ShemaSystem ptr: %p", this->instance_);
        return true;
    }
} // sdk