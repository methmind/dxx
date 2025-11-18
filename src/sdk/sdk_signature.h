//
// Created by sexey on 11.11.2025.
//

#ifndef DXX_DLC_SDK_SIGNATURE_H
#define DXX_DLC_SDK_SIGNATURE_H

namespace sdk::signature
{
    constexpr auto GET_VIEW_RENDER_INSTANCE_FUNC = "48 8D 05 ? ? ? ? C3 CC CC CC CC CC CC CC CC 85 D2 78";

    // Offset is (instance + 0x20F0)
    constexpr auto EGS_NUMBER_OF_ENTITIES = "80 FA ? 75 ? 8B 81";

    constexpr auto EGS_GET_BASE_ENTITY = "4C 8D 49 ? 81 FA ? ? ? ? 77";
}

#endif //DXX_DLC_SDK_SIGNATURE_H