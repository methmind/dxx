//
// Created by sexey on 19.11.2025.
//

#ifndef DXX_DLC_SDK_SCHEMA_SYSTEM_SINGLETON_H
#define DXX_DLC_SDK_SCHEMA_SYSTEM_SINGLETON_H

#include "memory/vmt_call.h"
#include "sdk/interface/sdk_schema_system.h"

namespace sdk::singleton
{
    /*
     * 48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 48 8B DA
     * void*(__thiscall*)(void* instance, const char* moduleName, void* unk);
    */
    constexpr auto FIND_TYPE_SCOPE_FOR_MODULE_VMT_INDEX = 13;

    class C_ShemaSystem
    {
    private:
        void* instance_;

    public:

        [[nodiscard]] iface::C_SchemaTypeScope* findTypeScopeForModule(const char* moduleName) const
        {
            return memory::vmt::call<iface::C_SchemaTypeScope*>(
                this->instance_, FIND_TYPE_SCOPE_FOR_MODULE_VMT_INDEX, moduleName, nullptr
            );
        }

        bool initialize();

        C_ShemaSystem() : instance_(nullptr) {}

        ~C_ShemaSystem() = default;
    };
} // sdk

#endif //DXX_DLC_SDK_SCHEMA_SYSTEM_SINGLETON_H