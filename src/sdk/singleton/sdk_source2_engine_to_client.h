//
// Created by sexey on 16.11.2025.
//

#ifndef DXX_DLC_SDK_SOURCE2_ENGINE_TO_CLIENT_H
#define DXX_DLC_SDK_SOURCE2_ENGINE_TO_CLIENT_H

#include <cstdint>

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

    class C_Source2EngineToClient
    {
    private:
        void* instance_;

    public:

        [[nodiscard]] bool isInGame() const;

        [[nodiscard]] uint32_t getLocalPlayerID() const;

        bool initialize();

        C_Source2EngineToClient() : instance_(nullptr) {}

        ~C_Source2EngineToClient() = default;
    };
} // sdk

#endif //DXX_DLC_SDK_SOURCE2_ENGINE_TO_CLIENT_H