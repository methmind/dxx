//
// Created by sexey on 10.12.2025.
//

#include "hook_impl_create_move.h"

#include "hook_impl_type.h"
#include "hook/hook_dispatcher.h"
#include "hook/hook_original_invoker.h"
#include "service_locator/service_locator.h"

namespace hook::impl
{
    void hkCreateMove(void* instance, const int32_t slot, const bool isActive)
    {
        MH_CALL_ORIGINAL(hkCreateMove)(instance, slot, isActive);

        C_ServiceLocator::getInstance<C_HookDispatcher>()->invoke(static_cast<hook_id_t>(hook_impl_type_e::CREATE_MOVE_RAW));
    }
} // hook