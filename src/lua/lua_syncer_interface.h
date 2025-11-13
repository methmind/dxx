//
// Created by sexey on 13.11.2025.
//

#ifndef DXX_DLC_LUA_SYNCER_INTERFACE_H
#define DXX_DLC_LUA_SYNCER_INTERFACE_H

#include <mutex>
#include <shared_mutex>

namespace lua
{
    using lua_sync_primitive_t = std::shared_mutex;

    using lua_state_locker_t = std::unique_lock<lua_sync_primitive_t>;

    class C_ILuaSyncerInterface
    {
    public:
        virtual lua_state_locker_t luaStateLocker() = 0;

        virtual ~C_ILuaSyncerInterface() = default;
    };
}

#endif //DXX_DLC_LUA_SYNCER_INTERFACE_H