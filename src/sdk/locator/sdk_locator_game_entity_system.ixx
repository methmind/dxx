//
// Created by sexey on 03.03.2026.
//
module;
#include "debug/debug_output.h"
#include "minhook/src/hde/hde64.h"

export module sdk.locator.game_entity_system;

import service.locator;
import sdk.engine.source2_client;

export namespace sdk
{
    void* GetGameEntitySystem()
    {
        const auto target = static_cast<uint8_t*>(C_ServiceLocator::Get<C_Source2Client>()->getGetNetworkCallbackQueue());
        if (!target) {
            return nullptr;
        }

        hde64s info;
        const auto length = hde64_disasm(target, &info);
        if (length == 0 || (info.flags & F_ERROR)) {
            return nullptr;
        }

        const auto offset = static_cast<int32_t>(info.disp.disp32);
        const auto instance = *reinterpret_cast<void**>(target + length + offset);

        dbg("C_GameEntitySystem instance: {:p}", instance);
        return instance;
    }
}
