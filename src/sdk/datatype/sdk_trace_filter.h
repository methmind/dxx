//
// Created by sexey on 24.12.2025.
//

#ifndef DXX_DLC_SDK_TRACE_FILTER_H
#define DXX_DLC_SDK_TRACE_FILTER_H

#include <array>
#include "sdk_base_entity.h"

namespace sdk::datatype
{
    struct trace_filter_s
    {
        void* vtable{};
        uint64_t m_uTraceMask;
        std::array<int64_t, 2> m_v1{};
        std::array<int32_t, 4> m_arrSkipHandles{};
        std::array<int16_t, 2> m_arrCollisions{};
        uint8_t m_v2{};
        uint8_t m_v3{};
        uint8_t m_v4{};
        uint8_t m_v5{};
        uint8_t collision{};

        explicit trace_filter_s(uint64_t traceMask, C_BaseEntity* entity, C_BaseEntity* player, int32_t layer)
        {
            this->m_uTraceMask = traceMask;
            this->m_v1[0] = m_v1[1] = 0;
            this->m_v2 = 15;
            this->m_v3 = layer;
            this->m_v4 = 0x49;

            this->collision = 0;

            if (entity)
            {
                m_arrSkipHandles[0] = entity->getIdentity()->getEntityHandle();
                m_arrSkipHandles[2] = entity->getIdentity()->getEntityHandle();
                m_arrCollisions[0] = entity->getCollisionComponent()->getCollisionMask();
            }

            if (player)
            {
                m_arrSkipHandles[0] = player->getIdentity()->getEntityHandle();
                m_arrSkipHandles[2] = player->getIdentity()->getEntityHandle();
                m_arrCollisions[0] = player->getCollisionComponent()->getCollisionMask();
            }
        }
    };
}

#endif //DXX_DLC_SDK_TRACE_FILTER_H