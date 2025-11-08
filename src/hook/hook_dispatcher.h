//
// Created by sexey on 07.11.2025.
//

#ifndef DXX_DLC_HOOK_DISPATCHER_H
#define DXX_DLC_HOOK_DISPATCHER_H

#include <any>
#include <functional>
#include <unordered_map>

#include "debug/debug_output.h"

namespace hook
{
    enum class hook_type_e
    {
        PRESENT = 0,
    };

    class C_HookDispatcher
    {
    private:
        std::unordered_map<hook_type_e, std::any> callback_;

    public:

        template<typename ... arg_t>
        void invoke(const hook_type_e type, const arg_t& ... args) const
        {
            const auto it = this->callback_.find(type);
            if (it == this->callback_.end()) {
                return;
            }

            try {
                const auto& callback = std::any_cast<std::function<void(arg_t...)>>(it->second);
                if (!callback) {
                    return;
                }

                callback(args...);
            } catch (const std::exception& ex) {
                dbg("Critical exception: %s", ex.what());
            }
        }

        template<typename ... arg_t>
        void subscribe(hook_type_e type, std::function<void(arg_t...)> callback)
        {
            this->callback_.emplace(type, std::any(callback));
        }

        C_HookDispatcher() = default;

        ~C_HookDispatcher() = default;
    };
} // hook

#endif //DXX_DLC_HOOK_DISPATCHER_H