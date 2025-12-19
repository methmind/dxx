//
// Created by sexey on 18.12.2025.
//

#include "hook_impl_get_matrices_for_view.h"

#include "hook_impl_type.h"
#include "hook/hook_dispatcher.h"
#include "hook/hook_original_invoker.h"
#include "service_locator/service_locator.h"

namespace hook::impl
{
    void* hkGetMatricesForView(void* renderGameSystem, void* viewRender, void* worldToView, void* viewToProjection, void* worldToProjection, void* worldToPixels)
    {
        const auto result = MH_CALL_ORIGINAL(hkGetMatricesForView)(renderGameSystem, viewRender, worldToView, viewToProjection, worldToProjection, worldToPixels);
        C_ServiceLocator::getInstance<C_HookDispatcher>()->invoke<void*, void*, void*, void*, void*, void*>(
            static_cast<hook_id_t>(hook_impl_type_e::GET_MATRICES_FOR_VIEW),
            renderGameSystem, viewRender, worldToView, viewToProjection, worldToProjection, worldToPixels
        );

        return result;
    }
} // hook