//
// Created by sexey on 18.11.2025.
//

#include "sdk_game_entity_system.h"

#include "sdk_source2_client.h"
#include "debug/debug_output.h"
#include "memory/pattern_scanner.h"
#include "minhook/src/hde/hde64.h"
#include "sdk/sdk_signature.h"
#include "service_locator/service_locator.h"

namespace sdk::singleton
{
    bool C_GameEntitySystem::findInstance()
    {
        const auto getNetworkChangeQueuePtr = reinterpret_cast<uint8_t*>(
            C_ServiceLocator::getInstance<C_Source2Client>()->getGetNetworkCallbackQueue()
        );

        if (!getNetworkChangeQueuePtr) {
            dbg("C_Source2Client::GetNetworkFieldChangeCallbackQueue unable to get ptr!");
            return false;
        }

        hde64s hs;
        const auto length = hde64_disasm(getNetworkChangeQueuePtr, &hs);
        if (!length || hs.flags & F_ERROR) {
            dbg("Unable to disasm C_Source2Client::GetNetworkFieldChangeCallbackQueue function!");
            return false;
        }

        this->instance_ = *reinterpret_cast<void**>(getNetworkChangeQueuePtr + hs.disp.disp32 + length);
        dbg("C_GameEntitySystem ptr: %p", this->instance_);

        return true;
    }

    bool C_GameEntitySystem::findMethods()
    {
        const auto module = GetModuleHandleA("client.dll");

        this->numberOfEntities_ = reinterpret_cast<number_of_entities_t>(
            memory::FindPattern(module,signature::EGS_NUMBER_OF_ENTITIES)
        );

        if (!this->numberOfEntities_) {
            dbg("Unable to find C_GameEntitySystem::GetHighestEntityIndex");
            return false;
        }

        this->getBaseEntity_ = reinterpret_cast<get_base_entity_t>(
            memory::FindPattern(module,signature::EGS_GET_BASE_ENTITY)
        );

        if (!this->getBaseEntity_) {
            dbg("Unable to find C_GameEntitySystem::GetBaseEntity");
            return false;
        }

        return true;
    }

    bool C_GameEntitySystem::initialize()
    {
        if (!findInstance()) {
            dbg("Unable to find instance of C_GameEntitySystem!");
            return false;
        }

        if (!findMethods()) {
            dbg("Unable to find method of C_GameEntitySystem!");
            return false;
        }

        return true;
    }
} // sdk