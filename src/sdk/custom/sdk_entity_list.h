//
// Created by sexey on 23.11.2025.
//

#ifndef DXX_DLC_SDK_ENTITY_LIST_H
#define DXX_DLC_SDK_ENTITY_LIST_H

#include <unordered_map>
#include <vector>

#include "sdk/interface/sdk_entity_instance.h"

namespace sdk::custom
{
    class C_EntityList
    {
    private:
        std::unordered_map<std::string, std::vector<iface::C_EntityInstance*>, xx_hashier_s> entities_;

        void syncEntities();

        void onLevelInit() { this->entities_.clear(); syncEntities(); }

        void onLevelShutdown() { this->entities_.clear(); }

        void onAddEntity(iface::C_EntityInstance* entity);

        void onRemoveEntity(iface::C_EntityInstance* entity);

    public:

        bool initialize();

        C_EntityList() = default;

        ~C_EntityList() = default;
    };
} // sdk

#endif //DXX_DLC_SDK_ENTITY_LIST_H