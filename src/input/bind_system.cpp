//
// Created by sexey on 10.11.2025.
//

#include "bind_system.h"

namespace input
{
    std::unique_ptr<C_KeyBindHandle> C_BindSystem::createBinding(const uint8_t key, bind_callback_t callback)
    {
        std::lock_guard lock_(this->mutex_);

        auto bind = std::make_unique<C_KeyBindHandle>(
            this->nextID_++,
            key,
            std::move(callback),
            weak_from_this()
        );

        this->bindings_[key].push_back(bind->callback_);
        return bind;
    }

    void C_BindSystem::removeBinding(const C_KeyBindHandle* bind)
    {
        std::lock_guard lock_(this->mutex_);

        const auto it = this->bindings_.find(bind->key_);
        if (it == this->bindings_.end()) {
            return;
        }

        std::erase_if(it->second, [&](const bind_callback_t& cb) {
            return cb.target<void(*)()>() == bind->callback_.target<void(*)()>();
        });

        if (it->second.empty()) {
            this->bindings_.erase(it);
        }
    }

    void C_BindSystem::invokeBindings(const uint8_t key)
    {
        std::shared_lock lock_(this->mutex_);

        const auto it = this->bindings_.find(key);
        if (it == this->bindings_.end()) {
            return;
        }

        for (auto& callback : it->second) {
            callback();
        }
    }
} // input