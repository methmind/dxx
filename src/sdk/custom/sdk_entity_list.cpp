//
// Created by sexey on 23.11.2025.
//

#include "sdk_entity_list.h"

#include "hook/hook_dispatcher.h"
#include "hook/impl/hook_impl_type.h"
#include "sdk/singleton/sdk_game_entity_system.h"

namespace sdk::custom
{
    void C_EntityList::syncEntities()
    {
        const auto entitySystem = C_ServiceLocator::getInstance<singleton::C_GameEntitySystem>();
        for (uint32_t i = 0, countOf = entitySystem->numberOfEntities(); i <= countOf; i++) {
            const auto entity = entitySystem->getBaseEntity<iface::C_EntityInstance>(i);
            if (!entity) {
                continue;
            }

            const auto entityInfo = entity->getClassInfo();
            if (!entityInfo) {
                continue;
            }

            this->entities_[entityInfo->getName()].emplace_back(entity);
        }
    }

    void C_EntityList::onAddEntity(iface::C_EntityInstance* entity)
    {
        const auto entityInfo = entity->getClassInfo();
        if (!entityInfo) {
            return;
        }

        dbg("[%s] new entity added!", entityInfo->getName());
        this->entities_[entityInfo->getName()].emplace_back(entity);
    }

    void C_EntityList::onRemoveEntity(iface::C_EntityInstance* entity)
    {
        const auto entityInfo = entity->getClassInfo();
        if (!entityInfo) {
            return;
        }

        const auto it = this->entities_.find(entityInfo->getName());
        if (it == this->entities_.end()) {
            return;
        }

        std::erase_if(it->second, [entity, entityInfo](auto& val) {
            dbg("[%s] entity removed!", entityInfo->getName());
            return val == entity;
        });
    }

    bool C_EntityList::initialize()
    {
        const auto hookDispatcher = C_ServiceLocator::getInstance<hook::C_HookDispatcher>();

        hookDispatcher->subscribe(
            static_cast<hook::hook_id_t>(hook::impl::hook_impl_type_e::ON_LEVEL_INIT),
            [this]{ onLevelInit(); }
        );

        hookDispatcher->subscribe(
            static_cast<hook::hook_id_t>(hook::impl::hook_impl_type_e::ON_LEVEL_SHUTDOWN),
            [this]{ onLevelShutdown(); }
        );

        hookDispatcher->subscribe<void*>(
            static_cast<hook::hook_id_t>(hook::impl::hook_impl_type_e::ON_ADD_ENTITY),
            [this](void* entity){ onAddEntity(static_cast<iface::C_EntityInstance*>(entity)); }
        );

        hookDispatcher->subscribe<void*>(
            static_cast<hook::hook_id_t>(hook::impl::hook_impl_type_e::ON_REMOVE_ENTITY),
            [this](void* entity){ onRemoveEntity(static_cast<iface::C_EntityInstance*>(entity)); }
        );

        return true;
    }
} // sdk