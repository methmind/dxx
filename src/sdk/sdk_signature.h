//
// Created by sexey on 11.11.2025.
//

#ifndef DXX_DLC_SDK_SIGNATURE_H
#define DXX_DLC_SDK_SIGNATURE_H

namespace sdk::signature
{
    // sub_181B8E900()
    constexpr auto GET_VIEW_RENDER_INSTANCE_FUNC = "48 8D 05 ? ? ? ? C3 CC CC CC CC CC CC CC CC 85 D2 78";

    // Offset is (instance + 0x20F0)
    constexpr auto EGS_NUMBER_OF_ENTITIES = "80 FA ? 75 ? 8B 81";

    // sub_18137FA10(__int64 a1, int a2)
    constexpr auto EGS_GET_BASE_ENTITY = "4C 8D 49 ? 81 FA ? ? ? ? 77";

    // "game_newmap" -> "mapname"
    constexpr auto ON_LEVEL_INIT_CALLBACK = "40 55 56 41 56 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 0D";

    // "map_shutdown"
    constexpr auto ON_LEVEL_SHUTDOWN_CALLBACK = "48 83 EC ? 48 8B 0D ? ? ? ? 48 8D 15 ? ? ? ? 45 33 C9 45 33 C0 48 8B 01 FF 50 ? 48 85 C0 74 ? 48 8B 0D ? ? ? ? 48 8B D0 4C 8B 01 41 FF 50 ? 48 83 C4";
}

#endif //DXX_DLC_SDK_SIGNATURE_H