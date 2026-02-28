//
// Created by sexey on 28.02.2026.
//
module;
#include <cstdint>

#include "hook/hook_invoker.h"

export module hook.impl.get_matrices_for_view;

import service.locator;
import hook.dispatcher;
import hook.type;

export namespace hook
{
    void* __attribute__((__fastcall__)) hkGetMatricesForView(void* renderGameSystem, void* viewRender, void* worldToView, void* viewToProjection, void* worldToProjection, void* worldToPixels)
    {
        const auto result = MH_CALL_ORIGINAL(hkGetMatricesForView)(renderGameSystem, viewRender, worldToView, viewToProjection, worldToProjection, worldToPixels);
        C_ServiceLocator::Get<C_HookDispatcher>()->invoke<static_cast<std::uint16_t>(hook_type_e::GET_MATRICES_FOR_VIEW)>(
            renderGameSystem, viewRender, worldToView, viewToProjection, worldToProjection, worldToPixels
        );

        return result;
    }
}
