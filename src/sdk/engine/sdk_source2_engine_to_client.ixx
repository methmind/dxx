//
// Created by sexey on 28.02.2026.
//
module;
#include <cstdint>

export module sdk.source2_engine_to_client;

import sdk.math.vector;

import memory.vmt;

namespace sdk
{
    /*
     * 40 53 48 83 EC ? 48 8B 05 ? ? ? ? 48 8D 0D ? ? ? ? 48 8B DA FF 90 ? ? ? ? 48 8B C3 48 83 C4 ? 5B C3 CC CC CC CC CC CC CC CC CC CC 48 8B 05
     *  void(__fastcall*)(void* instance, int32_t& playerSlot, int32_t splitScreenID);
     */
    constexpr auto GET_LOCAL_PLAYER_VMT_INDEX = 54;

    /*
     * 48 8B 05 ? ? ? ? 48 85 C0 74 ? 80 B8 ? ? ? ? ? 75 ? 83 B8 ? ? ? ? ? 7C
     * bool(__fastcall*)(void* instance);
    */
    constexpr auto IS_IN_GAME_VMT_INDEX = 39;

    /*
     * 48 8B 0D ? ? ? ? 48 8B 01 48 FF A0 ? ? ? ? CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC 48 8B 0D ? ? ? ? 48 85 C9
     * or
     * 48 89 5C 24 ? 57 48 83 EC ? 48 8B 0D ? ? ? ? 48 8B FA 48 8B 15
     * void(__fastcall*)(void* instance_but_prob_ignored, int32_t&, int32_t&);
     */
    constexpr auto GET_SCREEN_SIZE = 60;

    export class C_Source2EngineToClient
    {
    public:
        [[nodiscard]] bool isInGame()
        {
            return memory::vcall<bool(__attribute__((__fastcall__))*)(void*), IS_IN_GAME_VMT_INDEX>(this);
        }

        [[nodiscard]] int32_t getLocalPlayerID()
        {
            int32_t playerSlot = -1;
            memory::vcall<void(__attribute__((__fastcall__))*)(void*, int32_t*, int32_t), GET_LOCAL_PLAYER_VMT_INDEX>(
                this, &playerSlot, 0
            );

            return playerSlot + 1;
        }

        [[nodiscard]] vector2_t getScreenSize()
        {
            int32_t width = 0, height = 0;
            memory::vcall<void(__attribute__((__fastcall__))*)(void*, int32_t*, int32_t*), GET_SCREEN_SIZE>(
                this, &width, &height
            );

            return { static_cast<float>(width), static_cast<float>(height) };
        }
    };
}
