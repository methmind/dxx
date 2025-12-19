//
// Created by sexey on 16.11.2025.
//

#ifndef DXX_DLC_SDK_SOURCE2_ENGINE_TO_CLIENT_H
#define DXX_DLC_SDK_SOURCE2_ENGINE_TO_CLIENT_H

#include <cstdint>

#include "sdk/math/sdk_math_vector2.h"

namespace sdk::singleton
{
    /*
     * 40 53 48 83 EC ? 48 8B 05 ? ? ? ? 48 8D 0D ? ? ? ? 48 8B DA FF 90 ? ? ? ? 48 8B C3 48 83 C4 ? 5B C3 CC CC CC CC CC CC CC CC CC CC 48 8B 05
     *  void(__fastcall*)(void* instance, int32_t& playerSlot, int32_t splitScreenID);
     */
    constexpr auto GET_LOCAL_PLAYER_VMT_INDEX = 53;

    /*
     * 48 8B 05 ? ? ? ? 48 85 C0 74 ? 80 B8 ? ? ? ? ? 75 ? 83 B8 ? ? ? ? ? 7C
     * bool(__fastcall*)(void* instance);
    */
    constexpr auto IS_IN_GAME_VMT_INDEX = 38;

    /*
     * 48 8B 0D ? ? ? ? 48 8B 01 48 FF A0 ? ? ? ? CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC 48 8B 0D ? ? ? ? 48 85 C9
     * or
     * 48 89 5C 24 ? 57 48 83 EC ? 48 8B 0D ? ? ? ? 48 8B FA 48 8B 15
     * void(__fastcall*)(void* instance_but_prob_ignored, int32_t&, int32_t&);
     */
    constexpr auto GET_SCREEN_SIZE = 59;

    class C_Source2EngineToClient
    {
    private:
        void* instance_;

    public:

        [[nodiscard]] bool isInGame() const;

        [[nodiscard]] int32_t getLocalPlayerID() const;

        [[nodiscard]] math::vector2 getScreenSize() const;

        bool initialize();

        C_Source2EngineToClient() : instance_(nullptr) {}

        ~C_Source2EngineToClient() = default;
    };
} // sdk

#endif //DXX_DLC_SDK_SOURCE2_ENGINE_TO_CLIENT_H