//
// Created by sexey on 14.02.2026.
//
module;
#include <atomic>
#include <cassert>
#include <functional>
#include <memory>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "cs_plain_guarded.h"

export module hook.dispatcher;

namespace hook
{
    export using hook_id_t = uint16_t;

    export using hook_handle_t = uint64_t;

    export using hook_subscription_t = std::unique_ptr<void, std::function<void(void*)>>;

    struct callback_holder_base_s
    {
        size_t typeHash;
        hook_handle_t handle;

        virtual ~callback_holder_base_s() = default;

        explicit callback_holder_base_s(size_t hash, hook_handle_t handle) : typeHash(hash), handle(handle) {}
    };

    template<typename ... arg_t>
    struct callback_holder_s final : callback_holder_base_s
    {
        std::function<void(arg_t...)> fn;

        explicit callback_holder_s(std::function<void(arg_t...)> fn_, hook_handle_t handle)
            : callback_holder_base_s(typeid(void(arg_t...)).hash_code(), handle), fn(std::move(fn_)) {}
    };

    using callback_vector_t = std::vector<std::shared_ptr<callback_holder_base_s>>;

    export class C_HookDispatcher
    {
    public:
        ~C_HookDispatcher() = default;

        C_HookDispatcher() : globalEpoch_(0), nextHandle_(0) {}

        C_HookDispatcher(const C_HookDispatcher&) = delete;

        C_HookDispatcher& operator=(const C_HookDispatcher&) = delete;

        template<typename ... arg_t, typename fn_t>
        [[nodiscard]] hook_subscription_t subscribe(const hook_id_t id, fn_t&& callback)
        {
            auto func = std::function<void(arg_t...)>(std::forward<fn_t>(callback));
            const auto handle = this->nextHandle_.fetch_add(1, std::memory_order_relaxed);
            auto holder = std::make_shared<callback_holder_s<arg_t...>>(
                std::move(func),
                handle
            );

            (*this->callbacks_.lock())[id].push_back(std::move(holder));
            this->globalEpoch_.fetch_add(1, std::memory_order_release);

            return hook_subscription_t(
                reinterpret_cast<void*>(0xBADC0DE),
                [this, id, handle](void*) {
                    release(id, handle);
                }
            );
        }

        template<hook_id_t id, typename ... arg_t>
        __attribute__((always_inline)) void invoke(arg_t&& ... args)
        {
            thread_local std::vector<std::function<void(std::decay_t<arg_t>...)>> localCallbacks;
            thread_local uint64_t localEpoch = 0;

            if (localEpoch != this->globalEpoch_.load(std::memory_order_acquire)) [[unlikely]] {
                updateLocalCache<id, std::decay_t<arg_t>...>(localCallbacks, localEpoch);
            }

            for (const auto& callback : localCallbacks) {
                callback(args...);
            }
        }

        void release(hook_id_t id, hook_handle_t handle)
        {
            const auto globalMap = this->callbacks_.lock();
            const auto it = globalMap->find(id);
            if (it == globalMap->end()) {
                return;
            }

            std::erase_if(it->second, [handle](const std::shared_ptr<callback_holder_base_s>& callback) {
                return callback->handle == handle;
            });

            this->globalEpoch_.fetch_add(1, std::memory_order_release);
        }

    private:
        template<hook_id_t id, typename ... arg_t>
        __attribute__((noinline)) void updateLocalCache(std::vector<std::function<void(arg_t...)>>& localCache, uint64_t& localEpoch) const
        {
            localCache.clear();

            const auto globalMap = this->callbacks_.lock();
            if (const auto it = globalMap->find(id); it != globalMap->end()) {
                const size_t invokeHash = typeid(void(arg_t...)).hash_code();

                for (const auto& baseHolder : it->second) {
                    if (baseHolder->typeHash == invokeHash) {
                        auto* specific = static_cast<callback_holder_s<arg_t...>*>(baseHolder.get());
                        localCache.push_back(specific->fn);
                    } else {
                        assert(false && "Found inconsistent signature in hook dispatcher! This means that someone subscribed to the same hook id with a different callback signature. This is not allowed and may lead to undefined behavior!");
                    }
                }
            }

            localEpoch = this->globalEpoch_.load(std::memory_order_acquire);
        }

        libguarded::plain_guarded<std::unordered_map<hook_id_t, callback_vector_t>> callbacks_;
        std::atomic<uint64_t> globalEpoch_;
        std::atomic<hook_handle_t> nextHandle_;
    };
}
