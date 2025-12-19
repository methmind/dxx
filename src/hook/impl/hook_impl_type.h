//
// Created by sexey on 15.11.2025.
//

#ifndef DXX_DLC_HOOK_IMPL_TYPE_H
#define DXX_DLC_HOOK_IMPL_TYPE_H

#include <cstdint>

namespace hook::impl
{
    enum class hook_impl_type_e : uint16_t
    {
        UNKNOWN = 0,
        PRESENT, // HRESULT(__fastcall*)(IDXGISwapChain* self, UINT sync_interval, UINT flags)
        ON_RENDER_START, // void(__fastcall*)(void* instance)
        ON_LUA_DISPOSE, // void(__fastcall*)(const std::string_view& scriptID)
        ON_UPDATE, // void(__fastcall*)()
        ON_PRE_UPDATE, // void(__fastcall*)()
        ON_ADD_ENTITY, // void*(__fastcall*)(instance, void* entityInstance, int32_t handle
        ON_REMOVE_ENTITY, // void*(__fastcall*)(instance, void* entityInstance, int32_t handle
        ON_LEVEL_INIT, // void(__fastcall*)()
        ON_LEVEL_SHUTDOWN, // void(__fastcall*)()
        CREATE_MOVE_RAW, // void(__fastcall*)(void* instance, int32_t slot, bool isActive)
        CREATE_MOVE, // void(__fastcall*)(void* CUserCmd)
        GET_MATRICES_FOR_VIEW, // void*(__fastcall*)(void* renderGameSystem, void* viewRender, void* worldToView, void* viewToProjection, void* worldToProjection, void* worldToPixels)
    };
}

#endif //DXX_DLC_HOOK_IMPL_TYPE_H