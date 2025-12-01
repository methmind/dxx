//
// Created by sexey on 01.12.2025.
//

#include "sdk_base_game_system_factory.h"

#include "debug/debug_output.h"
#include "memory/pattern_scanner.h"
#include "minhook/src/hde/hde64.h"
#include "sdk/sdk_signature.h"

namespace sdk::singleton
{
    void* C_BaseGameSystemFactory::find(const std::string_view& name) const
    {
        for (auto it = static_cast<C_IGameSystemFactory*>(this->instance_); it; it = it->next) {
            if (it->name == name) {
                return it->gameSystem;
            }
        }

        return nullptr;
    }

    bool C_BaseGameSystemFactory::initialize()
    {
        const auto movOpcode = static_cast<uint8_t*>(
            memory::FindPattern(GetModuleHandleA("client.dll"), signature::GAME_SYSTEM_FACTORY_MOV_OPCODE)
        );

        if (!movOpcode) {
            dbg("Unable to find C_BaseGameSystemFactory global pointer!");
            return false;
        }

        hde64s hs;
        const auto length = hde64_disasm(movOpcode, &hs);
        if (!length || hs.flags & F_ERROR) {
            dbg("Unable to disasm \"mov rbx, cs:g_CBaseGameSystemFactory!\"");
            return false;
        }

        this->instance_ = *reinterpret_cast<void**>(movOpcode + hs.disp.disp32 + length);
        dbg("C_BaseGameSystemFactory ptr: %p", this->instance_);

        return true;
    }
} // sdk