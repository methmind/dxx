//
// Created by sexey on 27.02.2026.
//
module;

export module sdk.panorama.ui_engine_source2;

import memory.vmt;
import sdk.panorama.image_resource_manager;

namespace sdk
{
    /*
     * 48 8B 81 ? ? ? ? C3 CC CC CC CC CC CC CC CC 48 8B C4
     * void*(__fastcall*)(void* instance);
     */
    constexpr auto GET_IMAGE_RESOURCE_MANAGER_VMT = 23;

    export class C_UIEngineSource2
    {
    public:
        [[nodiscard]] C_ImageResourceManager* getImageResourceManager()
        {
            return memory::vcall<C_ImageResourceManager*(__attribute__((__fastcall__))*)(void*), GET_IMAGE_RESOURCE_MANAGER_VMT>(this);
        }
    };
}