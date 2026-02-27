//
// Created by sexey on 27.02.2026.
//
module;
#include <d3d11.h>

#include "memory/offset.h"

export module sdk.panorama.image_proxy_source;

import memory.vmt;

export namespace sdk
{
    /*
     * 48 8B 51 ? 48 85 D2 74 ? 48 8B 42
     * void*(__fastcall*)(void* instance);
     */
    constexpr auto GET_SOURCE2_UI_TEXTURE_VMT = 4;

    class C_D3D11TextureWrapper
    {
    public:
        OFFSET(ID3D11ShaderResourceView*, getTextureSRV, 0x10);
    };

    class C_Source2UITexture
    {
    public:
        OFFSET(C_D3D11TextureWrapper**, getD3DWrapper, 0x28);
    };

    class C_ImageProxySource
    {
    public:
        [[nodiscard]] ID3D11ShaderResourceView* getSharedResourceView()
        {
            const auto uiTexture = memory::vcall<C_Source2UITexture*(__attribute__((__fastcall__))*)(void*), 4>(this);//imageData->getUITexture();
            if (!uiTexture) {
                return nullptr;
            }

            const auto textureData = *uiTexture->getD3DWrapper();
            if (!textureData) {
                return nullptr;
            }

            return textureData->getTextureSRV();
        }
    };
}