//
// Created by sexey on 24.12.2025.
//

#include "sdk_phys2_world.h"
#include "debug/debug_output.h"
#include "sdk/sdk_signature.h"
#include "memory/pattern_scanner.h"
#include "minhook/src/hde/hde64.h"

namespace sdk::singleton
{
    bool C_Phys2World::findInstance(HMODULE clientModule)
    {
        const auto movR12 = static_cast<uint8_t*>(
            memory::FindPattern(clientModule, signature::MOV_R12_GLOBAL_VPHYS2_WORLD)
        );

        if (!movR12) {
            dbg("Unable to find \"mov r12, cs:g_CPhys2World\" pattern!");
            return false;
        }

        hde64s hs;
        const auto length = hde64_disasm(movR12, &hs);
        if (!length || hs.flags & F_ERROR) {
            dbg("Unable to disasm \"mov r12, cs:g_CPhys2World\"");
            return false;
        }

        this->instance_ = *reinterpret_cast<void**>(movR12 + hs.disp.disp32 + length);
        dbg("g_CPhys2World ptr: %p", this->instance_);

        return true;
    }

    bool C_Phys2World::resolveFunctions(HMODULE clientModule)
    {
        if (this->traceShapeFn_ = reinterpret_cast<trace_shape_t>(
            memory::FindPattern(clientModule, signature::VPHYS2_WORLD_TRACE_SHAPE_FUNC)); !this->traceShapeFn_) {
            dbg("Unable to find C_Phys2World::TraceShape function!");
            return false;
        }

        return true;
    }

    bool C_Phys2World::traceShape(datatype::ray_s* ray_t, math::vec3_s* start, math::vec3_s* end,
        datatype::trace_filter_s* filter, datatype::C_GameTrace* game_trace) const
    {
        return this->traceShapeFn_(this->instance_, ray_t, start, end, filter, game_trace);
    }

    bool C_Phys2World::initialize()
    {
        const auto clientModule = GetModuleHandleA("client.dll");
        if (!findInstance(clientModule)) {
            dbg("Unable to find instance of C_Phys2World");
            return false;
        }

        if (!resolveFunctions(clientModule)) {
            dbg("Unable to resolve C_Phys2World functions!");
            return false;
        }

        return true;
    }
} // sdk