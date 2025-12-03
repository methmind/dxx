//
// Created by sexey on 07.11.2025.
//

#ifndef DXX_DLC_HOOK_DISPATCHER_H
#define DXX_DLC_HOOK_DISPATCHER_H

#include <functional>
#include <xenium/harris_michael_hash_map.hpp>
#include <xenium/reclamation/generic_epoch_based.hpp>
#include "cs_lr_guarded.h"

#include "debug/debug_output.h"

namespace hook
{
    constexpr auto MAX_BUCKET_SIZE = 32;

    using hook_id_t = uint16_t;

    class C_HookDispatcher
    {
    private:
        struct callback_holder_base_s {
            virtual ~callback_holder_base_s() = default;
        };

        template<typename ... arg_t>
        struct callback_holder_s final : callback_holder_base_s {
            std::function<void(arg_t...)> fn;

            explicit callback_holder_s(std::function<void(arg_t...)> fn_) : fn(std::move(fn_)) {}

            ~callback_holder_s() override = default;
        };

        using callback_vector_t = std::vector<std::shared_ptr<callback_holder_base_s>>;

        using callback_map_t = xenium::harris_michael_hash_map<
            hook_id_t, libguarded::lr_guarded<callback_vector_t>,
            xenium::policy::reclaimer<xenium::reclamation::epoch_based<>>,
            xenium::policy::hash<std::hash<hook_id_t>>,
            std::equal_to<hook_id_t>,
            xenium::policy::buckets<MAX_BUCKET_SIZE>
        >;

        mutable callback_map_t callbacks_{};

    public:

        template<typename ... arg_t>
        void subscribe(const hook_id_t type, std::invocable<arg_t...> auto callback)
        {
            auto [it, _] = this->callbacks_.get_or_emplace(type);

            using func_t = std::function<void(arg_t...)>;
            auto func = func_t(std::forward<decltype(callback)>(callback));

            it->second.modify([fn = std::move(func)](callback_vector_t& storage) {
                storage.emplace_back(std::make_shared<callback_holder_s<arg_t...>>(std::move(fn)));
            });

            dbg("New listener subscribed to: %d", type);
        }

        template<typename ... arg_t>
        void invoke(const hook_id_t type, const arg_t& ... args) const
        {
            const auto it = this->callbacks_.find(type);
            if (it == this->callbacks_.end()) [[unlikely]] {
                return;
            }

            const auto callbacks = it->second.lock_shared();

            for (const auto& holderBase : *callbacks) {
                try {
                    auto* holder = static_cast<callback_holder_s<arg_t...>*>(holderBase.get());
                    holder->fn(args...);
                } catch (const std::exception& ex) {
                    dbg("Critical exception in callback: %s", ex.what());
                }
            }
        }

        void clear() const;

        C_HookDispatcher() = default;

        ~C_HookDispatcher() = default;
    };
} // hook

#endif //DXX_DLC_HOOK_DISPATCHER_H