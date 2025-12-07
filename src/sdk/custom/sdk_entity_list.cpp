//
// Created by sexey on 23.11.2025.
//

#include "sdk_entity_list.h"

#include "hook/hook_dispatcher.h"
#include "hook/impl/hook_impl_type.h"
#include "memory/pattern_scanner.h"
#include "sdk/singleton/sdk_game_entity_system.h"

namespace sdk::custom
{
    bool C_EntityList::IsProhibitedEntity(const std::string_view& entityClassName)
    {
        return SCENE_ENTITY_NAME == entityClassName;
    }

    std::optional<std::string_view> C_EntityList::GetTruncatedIdentityName(const std::string_view& identityName)
    {
        for (const auto&[pattern, truncatedName] : SPECIAL_ENTITY_IDENTITY_LIST) {
            if (identityName.find(pattern) != std::string::npos) {
                return truncatedName;
            }
        }

        return std::nullopt;
    }

    std::optional<std::string_view> C_EntityList::GetSpecialEntityName(iface::C_EntityInstance* entity)
    {
        const auto classInfo = entity->getClassInfo();
        if (!classInfo) {
            return std::nullopt;
        }

        const auto className = classInfo->getName();
        if (!className) {
            return std::nullopt;
        }

        return GetTruncatedIdentityName(className);
    }

    void C_EntityList::removeFromSpecialEntity(iface::C_EntityInstance* entity)
    {
        const auto truncatedIdentityName = GetSpecialEntityName(entity);
        if (!truncatedIdentityName.has_value()) {
            return;
        }

        const auto it = this->entities_.find(truncatedIdentityName->data());
        if (it == this->entities_.end()) {
            return;
        }

        std::erase_if(it->second, [entity](auto& val) {
            return val == entity;
        });
    }

    void C_EntityList::addToSpecialCategory(iface::C_EntityInstance* entity)
    {
        const auto truncatedIdentityName = GetSpecialEntityName(entity);
        if (!truncatedIdentityName.has_value()) {
            return;
        }

        this->entities_[truncatedIdentityName->data()].emplace_back(entity);
    }

    void C_EntityList::syncEntities()
    {
        C_ServiceLocator::getInstance<singleton::C_GameEntitySystem>()->iterateEntities(
            [this](iface::C_EntityInstance* entity) {
                onAddEntity(entity);
            }
        );
    }

    void C_EntityList::onAddEntity(iface::C_EntityInstance* entity)
    {
        const auto entityInfo = entity->getClassInfo();
        if (!entityInfo) {
            return;
        }

        if (IsProhibitedEntity(entityInfo->getName())) {
            return;
        }

        addToSpecialCategory(entity);
        this->entities_[entityInfo->getName()].emplace_back(entity);
    }

    void C_EntityList::onRemoveEntity(iface::C_EntityInstance* entity)
    {
        const auto entityInfo = entity->getClassInfo();
        if (!entityInfo) {
            return;
        }

        if (IsProhibitedEntity(entityInfo->getName())) {
            return;
        }

        const auto it = this->entities_.find(entityInfo->getName());
        if (it == this->entities_.end()) {
            return;
        }

        std::erase_if(it->second, [entity](auto& val) {
            return val == entity;
        });

        removeFromSpecialEntity(entity);
    }

    size_t C_EntityList::count(const std::string& entityClassName) const
    {
        if (const auto it = this->entities_.find(entityClassName); it != this->entities_.end()) {
            return it->second.size();
        }

        return 0;
    }

    C_EntityList::instance_list_t& C_EntityList::find(const std::string& name)
    {
        if (const auto it = this->entities_.find(name); it != this->entities_.end()) {
            return it->second;
        }

        static instance_list_t empty;
        return empty;
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

        hookDispatcher->subscribe<iface::C_EntityInstance*>(
            static_cast<hook::hook_id_t>(hook::impl::hook_impl_type_e::ON_ADD_ENTITY),
            [this](iface::C_EntityInstance* entity){ onAddEntity(entity); }
        );

        hookDispatcher->subscribe<iface::C_EntityInstance*>(
            static_cast<hook::hook_id_t>(hook::impl::hook_impl_type_e::ON_REMOVE_ENTITY),
            [this](iface::C_EntityInstance* entity){ onRemoveEntity(entity); }
        );

        syncEntities();
        return true;
    }
} // sdk