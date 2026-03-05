//
// Created by sexey on 04.03.2026.
//
module;
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "constexpr-xxh3.h"

export module sdk.entity_list;

import xxhash.wrapper;
import service.locator;

import hook.dispatcher;
import hook.type;

import sdk.entity.instance;
import sdk.schema.datatype;

namespace sdk
{
    consteval uint64_t cx_entity_hash(const std::string_view& name)
    {
        return constexpr_xxh3::XXH3_64bits_const(name.data(), name.size());
    }

    constexpr std::array<uint64_t, 1> IGNORED_ENTITIES =
    {
        cx_entity_hash("C_DOTASceneEntity")
    };

    constexpr std::array<std::pair<uint64_t, std::string_view>, 4> SPECIAL_CATEGORIES =
    {
        std::pair{ cx_entity_hash("C_DOTABaseAbility"),       "ability" },
        std::pair{ cx_entity_hash("C_DOTAPlayerController"),  "player"  },
        std::pair{ cx_entity_hash("C_DOTA_BaseNPC"),          "npc"     },
        std::pair{ cx_entity_hash("C_DOTA_BaseNPC_Hero"),     "hero"    },
    };

    export class C_EntityList
    {
    public:
        using instance_list_t = std::vector<void*>;

        ~C_EntityList() = default;

        C_EntityList()
        {
            const auto dispatcher = C_ServiceLocator::Get<hook::C_HookDispatcher>();

            this->onEntityAddSubscription_ = dispatcher->subscribe<void*>(
                static_cast<hook::hook_id_t>(hook::hook_type_e::ON_ENTITY_ADD),
                [this](void* raw) { onEntityAdd(static_cast<C_EntityInstance*>(raw)); }
            );

            this->onEntityRemoveSubscription_ = dispatcher->subscribe<void*>(
                static_cast<hook::hook_id_t>(hook::hook_type_e::ON_ENTITY_REMOVE),
                [this](void* raw) { onEntityRemove(static_cast<C_EntityInstance*>(raw)); }
            );

            this->onLevelShutdownSubscription_ = dispatcher->subscribe(
                static_cast<hook::hook_id_t>(hook::hook_type_e::ON_LEVEL_SHUTDOWN),
                [this] { entities_.clear(); }
            );
        }

        [[nodiscard]] size_t count(std::string_view entityClassName) const
        {
            const auto it = this->entities_.find(entityClassName);
            return it != this->entities_.end() ? it->second.size() : 0;
        }

        instance_list_t& find(std::string_view entityClassName)
        {
            if (const auto it = this->entities_.find(entityClassName); it != this->entities_.end()) {
                return it->second;
            }

            static instance_list_t empty;
            return empty;
        }

    private:
        [[nodiscard]] static const C_SchemaClassInfo* GetValidatedClassInfo(C_EntityInstance* entity)
        {
            const auto* classInfo = entity->getClassInfo();
            assert(classInfo && "C_EntityInstance::getClassInfo == nullptr. Check signature!");

            return classInfo;
        }

        [[nodiscard]] static bool IsIgnoredEntity(const std::string_view& className)
        {
            const auto hash = xx_hasher_s::cx_hash(className);
            return std::ranges::any_of(IGNORED_ENTITIES, [hash](uint64_t h) { return h == hash; });
        }

        [[nodiscard]] static std::string_view findSpecialCategory(const C_SchemaClassInfo* classInfo)
        {
            for (const auto* info = classInfo->getNextInheritance()->instance;
                 info->getNextInheritance();
                 info = info->getNextInheritance()->instance)
            {
                const auto hash = xx_hasher_s::cx_hash(info->getName());
                const auto it = std::ranges::find_if(SPECIAL_CATEGORIES,
                    [hash](const auto& cat) { return cat.first == hash; });

                if (it != SPECIAL_CATEGORIES.end()) {
                    return it->second;
                }
            }

            return {};
        }

        void eraseEntity(const std::string_view& category, void* entity)
        {
            if (const auto it = this->entities_.find(category); it != this->entities_.end()) {
                std::erase(it->second, entity);
            }
        }

        void onEntityAdd(C_EntityInstance* entity)
        {
            const auto* classInfo = GetValidatedClassInfo(entity);
            if (!classInfo || IsIgnoredEntity(classInfo->getName())) {
                return;
            }

            if (const auto category = findSpecialCategory(classInfo); !category.empty()) {
                this->entities_[std::string(category)].emplace_back(entity);
            }

            this->entities_[classInfo->getName()].emplace_back(entity);
        }

        void onEntityRemove(C_EntityInstance* entity)
        {
            const auto* classInfo = GetValidatedClassInfo(entity);
            if (!classInfo || IsIgnoredEntity(classInfo->getName())) {
                return;
            }

            if (const auto category = findSpecialCategory(classInfo); !category.empty()) {
                eraseEntity(category, entity);
            }

            eraseEntity(classInfo->getName(), entity);
        }

        std::unordered_map<std::string, instance_list_t, xx_hasher_s, std::equal_to<>> entities_;

        hook::hook_subscription_t onEntityAddSubscription_;
        hook::hook_subscription_t onEntityRemoveSubscription_;
        hook::hook_subscription_t onLevelShutdownSubscription_;
    };
}
