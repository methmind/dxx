//
// Created by sexey on 14.02.2026.
//
module;
#include "MinHook.h"
#include "debug/debug_output.h"

export module hook.container;

namespace hook
{
    export class C_HookContainer
    {
    public:
        ~C_HookContainer()
        {
            MH_Uninitialize();
        }

        C_HookContainer() : dummy_(0) {}

        static uint8_t Create(void* target, void* detour)
        {
            return MH_CreateHook(target, detour, nullptr);
        }

        [[nodiscard]] bool initialize() const
        {
            if (const uint8_t err = MH_Initialize(); err != MH_OK) {
                dbg("MH_Initialize got:err = {}", err);
                return false;
            }

            (void)this->dummy_;
            return true;
        }

        [[nodiscard]] bool enable() const
        {
            if (const uint8_t err = MH_EnableHook(MH_ALL_HOOKS); err != MH_OK) {
                dbg("MH_EnableHook got:err = {}", err);
                return false;
            }

            (void)this->dummy_;
            return true;
        }

    private:
        uint8_t dummy_; // Просто для подавления предупреждения о том, что нужно использовать static.
    };
}
