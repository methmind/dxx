//
// Created by sexey on 23.11.2025.
//

#ifndef DXX_DLC_SDK_ENTITY_LIST_H
#define DXX_DLC_SDK_ENTITY_LIST_H

#include <unordered_map>
#include <vector>
#include <array>

#include "sdk/interface/sdk_entity_instance.h"

namespace sdk::custom
{
    constexpr std::string_view SCENE_ENTITY_NAME = "C_DOTASceneEntity";

    constexpr std::array<std::pair<std::string_view, std::string_view>, 2> SPECIAL_ENTITY_IDENTITY_LIST = {
        std::make_pair("DOTA_Unit_Hero", "hero"),
        std::make_pair("C_DOTA_BaseNPC_Creep", "creep")
    };

    class C_EntityList
    {
    public:
        using instance_list_t = std::vector<iface::C_EntityInstance*>;

    private:
        std::unordered_map<std::string, instance_list_t, xx_hashier_s, std::equal_to<>> entities_;

        static bool IsProhibitedEntity(const std::string_view& entityClassName);

        static std::optional<std::string_view> GetTruncatedIdentityName(const std::string_view& identityName);

        static std::optional<std::string_view> GetSpecialEntityName(iface::C_EntityInstance* entity);

        void removeFromSpecialEntity(iface::C_EntityInstance* entity);

        void addToSpecialCategory(iface::C_EntityInstance* entity);

        void syncEntities();

        void onLevelInit() { this->entities_.clear(); syncEntities(); }

        void onLevelShutdown() { this->entities_.clear(); }

        void onAddEntity(iface::C_EntityInstance* entity);

        void onRemoveEntity(iface::C_EntityInstance* entity);

    public:

        size_t count(const std::string& entityClassName) const;

        instance_list_t& find(const std::string& name);

        bool initialize();

        C_EntityList() = default;

        ~C_EntityList() = default;
    };
} // sdk

#endif //DXX_DLC_SDK_ENTITY_LIST_H