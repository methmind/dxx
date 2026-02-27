//
// Created by sexey on 27.02.2026.
//
module;

export module sdk.panorama.ui_engine;

import memory.vmt;
import sdk.panorama.ui_engine_source2;

namespace sdk
{
    /*
     * 48 8B 41 ? C3 CC CC CC CC CC CC CC CC CC CC CC 48 89 5C 24 ? 48 89 6C 24
     * void*(__fastcall*)(void* instance);
     */
    constexpr auto GET_UI_ENGINE_SOURCE2_VMT = 13;

    export class C_PanoramaUIEngine
    {
    public:
        [[nodiscard]] C_UIEngineSource2* getUIEngineSource2()
        {
            return memory::vcall<C_UIEngineSource2*(__attribute__((__fastcall__))*)(void*), GET_UI_ENGINE_SOURCE2_VMT>(this);
        }
    };
}