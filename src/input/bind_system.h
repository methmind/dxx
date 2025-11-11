//
// Created by sexey on 10.11.2025.
//

#ifndef DXX_DLC_BIND_SYSTEM_H
#define DXX_DLC_BIND_SYSTEM_H

#include <cstdint>
#include <functional>
#include <unordered_map>
#include <memory>
#include <shared_mutex>

namespace input
{
    using bind_callback_t = std::function<void()>;

    class C_BindSystem;

    class C_KeyBindHandle
    {
    private:
        uint64_t id_;
        uint8_t key_;
        bind_callback_t callback_;
        std::weak_ptr<C_BindSystem> parent_;

    public:

        explicit C_KeyBindHandle(const uint64_t id, const uint8_t key, bind_callback_t callback,
            std::weak_ptr<C_BindSystem> parent
        ) : id_(id), key_(key), callback_(std::move(callback)), parent_(std::move(parent)) {}

        ~C_KeyBindHandle();

        friend class C_BindSystem;
    };

    class C_BindSystem : std::enable_shared_from_this<C_BindSystem>
    {
    private:
        std::shared_mutex mutex_;
        std::unordered_map<uint8_t, std::vector<bind_callback_t>> bindings_;
        uint64_t nextID_;

    public:

        std::unique_ptr<C_KeyBindHandle> createBinding(uint8_t key, bind_callback_t callback);

        void removeBinding(const C_KeyBindHandle* bind);

        void invokeBindings(uint8_t key);

        C_BindSystem() : nextID_(0) {}

        ~C_BindSystem() = default;
    };

    inline C_KeyBindHandle::~C_KeyBindHandle()
    {
        this->parent_.lock()->removeBinding(this);
    }
} // input

#endif //DXX_DLC_BIND_SYSTEM_H