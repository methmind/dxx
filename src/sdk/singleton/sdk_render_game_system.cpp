//
// Created by sexey on 01.12.2025.
//

#include "sdk_render_game_system.h"

#include "sdk_base_game_system_factory.h"
#include "debug/debug_output.h"
#include "memory/pattern_scanner.h"
#include "sdk/sdk_signature.h"
#include "service_locator/service_locator.h"

namespace sdk::singleton
{
    bool C_RenderGameSystem::initialize()
    {
        if (this->instance_ = C_ServiceLocator::getInstance<C_BaseGameSystemFactory>()->find(RENDER_GAME_SYSTEM_SID);
            !this->instance_) {
            dbg("Unable to find C_RenderGameSystem!");
            return false;
        }

        this->getMatricesForView_ = reinterpret_cast<FARPROC>(memory::FindPattern(GetModuleHandleA("client.dll"), signature::GET_MATRICES_FOR_VIEW));
        if (!this->getMatricesForView_) {
            dbg("Unable to find C_RenderGameSystem::GetMatricesForView!");
            return false;
        }

        dbg("C_RenderGameSystem ptr: %p", this->instance_);
        return true;
    }
} // sdk