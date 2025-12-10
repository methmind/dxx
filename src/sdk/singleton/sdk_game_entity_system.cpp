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
        //identitiesChunks_ offset is instance ptr + 0x10
        this->identitiesChunks_ = reinterpret_cast<custom::entity_identities_chunk_s**>(static_cast<uint8_t*>(this->instance_) + 0x10);

        dbg("C_GameEntitySystem ptr: %p", this->instance_);
        return true;
    }

    void* C_GameEntitySystem::getBaseEntityImpl(const int32_t index) const
    {
        if (index < 0 || index >= custom::MAX_TOTAL_ENTITIES) {
            return nullptr;
        }

        const auto chunkIndex = index / custom::MAX_ENTITIES_IN_CHUNK;
        const auto chunk = this->identitiesChunks_[chunkIndex];
        if (!chunk) {
            return nullptr;
        }

        const auto indexInChunk = index % custom::MAX_ENTITIES_IN_CHUNK;
        const auto identity = &chunk->identities[indexInChunk];
        if (const util::C_BaseEntityHandle identityHandle(identity->getEntityHandle()); identityHandle.getEntryIndex() != index) {
            return nullptr;
        }

        return identity->getAssignedEntity();
    }

    bool C_GameEntitySystem::initialize()
    {
        if (!findInstance()) {
            dbg("Unable to find instance of C_GameEntitySystem!");
            return false;
        }

        if (this->getPlayerController_ = reinterpret_cast<get_player_controller_t>(
            memory::FindPattern(GetModuleHandleA("client.dll"), signature::GET_PLAYER_CONTROLLER_FUNC)); this->getPlayerController_) {
            dbg("Unable to find C_GameEntitySystem::GetPlayerController function!");
            return false;
        }

        return true;
    }
} // sdk