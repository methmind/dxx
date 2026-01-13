//
// Created by sexey on 07.11.2025.
//

#include "hook_dispatcher.h"

namespace hook
{
    void C_HookDispatcher::clear() const
    {
        for (auto it = this->callbacks_.begin();
            it != this->callbacks_.end();
            it = this->callbacks_.erase(it)) {}
    }
} // hook