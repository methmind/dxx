//
// Created by sexey on 10.11.2025.
//

#include "bind_system.h"

namespace input
{
    C_KeyBindHandle::~C_KeyBindHandle()
    {
        if (const auto parent = this->parent_.lock(); parent) {
            parent->removeBinding(this);
        }
    }

    std::unique_ptr<C_KeyBindHandle> C_BindSystem::createBinding(const uint8_t key, bind_callback_t callback)
    {
        const auto binds = this->bindings_.lock();
        auto newBind = std::make_unique<C_KeyBindHandle>(
            this->nextID_++,
            key,
            std::move(callback),
            weak_from_this()
        );

        (*binds)[key].push_back(newBind->callback_);
        return newBind;
    }

    void C_BindSystem::removeBinding(const C_KeyBindHandle* bind)
    {
        const auto binds = this->bindings_.lock();
        const auto it = binds->find(bind->key_);
        if (it == binds->end()) {
            return;
        }

        std::erase_if(it->second, [&](const bind_callback_t& cb) {
            return cb.target<void(*)()>() == bind->callback_.target<void(*)()>();
        });

        if (it->second.empty()) {
            binds->erase(it);
        }
    }

    void C_BindSystem::invokeBindings(const uint8_t key) const
    {
        const auto binds = this->bindings_.lock_shared();
        const auto it = binds->find(key);
        if (it == binds->end()) {
            return;
        }

        for (auto& callback : it->second) {
            callback();
        }
    }
} // input