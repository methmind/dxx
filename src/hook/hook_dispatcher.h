//
// Created by sexey on 07.11.2025.
//

#ifndef DXX_DLC_HOOK_DISPATCHER_H
#define DXX_DLC_HOOK_DISPATCHER_H

#include <functional>
#include <xenium/harris_michael_hash_map.hpp>
#include <xenium/reclamation/generic_epoch_based.hpp>

#include "debug/debug_output.h"

namespace hook
{
    constexpr auto MAX_BUCKET_SIZE = 32;

    using hook_sid_t = std::string;

    class C_HookDispatcher
    {
    private:
        struct callback_holder_base_s {
            const std::type_info* type;

            [[nodiscard]] virtual bool isSameType(const std::type_info& other) const = 0;

            [[nodiscard]] virtual bool isUnsafeAllowed() const { return false; }

            explicit callback_holder_base_s(const std::type_info& type) : type(&type) {}

            virtual ~callback_holder_base_s() = default;
        };

        template<typename ... arg_t>
        struct callback_holder_s final : callback_holder_base_s {
            std::function<void(arg_t...)> fn;
            bool isAllowUnsafeCall;

            [[nodiscard]] bool isSameType(const std::type_info& other) const override {
                return *type == other;
            }

            [[nodiscard]] bool isUnsafeAllowed() const override {
                return this->isAllowUnsafeCall;
            }

            explicit callback_holder_s(const bool allowUnsafeCall, std::function<void(arg_t...)> fn_) :
                callback_holder_base_s(typeid(void(*)(arg_t...))), fn(std::move(fn_)),
                isAllowUnsafeCall(allowUnsafeCall) {}

            ~callback_holder_s() override = default;
        };

        using callback_vector_t = std::vector<std::shared_ptr<callback_holder_base_s>>;
        using callback_storage_t = std::shared_ptr<callback_vector_t>;

        using callback_map_t = xenium::harris_michael_hash_map<
            hook_sid_t, std::atomic<callback_storage_t>,
            xenium::policy::reclaimer<xenium::reclamation::epoch_based<>>,
            xenium::policy::hash<std::hash<hook_sid_t>>,
            std::equal_to<hook_sid_t>,
            xenium::policy::buckets<MAX_BUCKET_SIZE>
        >;

        mutable callback_map_t callbacks_{};

    public:

        template<typename ... arg_t>
        void subscribe(const hook_sid_t& type, bool allowUnsafe, std::invocable<arg_t...> auto callback)
        {
            auto [it, _] = this->callbacks_.get_or_emplace(type);
            auto current = it->second.load(std::memory_order_acquire);

            while (true) {
                auto newSlice = current ?
                    std::make_shared<callback_vector_t>(*current) :
                    std::make_shared<callback_vector_t>();

                newSlice->push_back(
                    std::make_shared<callback_holder_s<arg_t...>>(
                        allowUnsafe, std::function<void(arg_t...)>(std::move(callback))
                    )
                );

                if (it->second.compare_exchange_weak(current, newSlice,
                    std::memory_order_release,
                    std::memory_order_acquire)
                ) {
                    break;
                }
            }

            dbg("New listener subscribed to: %s", type.c_str());
        }

        template<typename ... arg_t>
        void invoke(const hook_sid_t& type, const arg_t& ... args) const
        {
            const auto it = this->callbacks_.find(type);
            if (it == this->callbacks_.end()) {
                return;
            }

            const auto callbacks = it->second.load(std::memory_order_acquire);
            if (!callbacks || callbacks->empty()) {
                return;
            }

            const auto& targetTypeID = typeid(void(*)(arg_t...)); // prob. better than std::dynamic_pointer_cast
            for (const auto& holderBase : *callbacks) {
                try {
                    if (!holderBase->isUnsafeAllowed() && !holderBase->isSameType(targetTypeID)) {
                        throw std::runtime_error("Callback type mismatch!");
                    }

                    auto holder = std::static_pointer_cast<callback_holder_s<arg_t...>>(holderBase);
                    if (holder && holder->fn) {
                        holder->fn(args...);
                    }

                    //dbg("Listener for hook: %s invoked", type.c_str());
                } catch (const std::exception& ex) {
                    dbg("Critical exception in callback: %s", ex.what());
                }
            }
        }

        C_HookDispatcher() = default;

        ~C_HookDispatcher() = default;
    };
} // hook

#endif //DXX_DLC_HOOK_DISPATCHER_H