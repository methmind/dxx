//
// Created by sexey on 26.02.2026.
//
module;
#include <cstdint>
#include <d3d11.h>
#include <memory>
#include <vector>

#include "debug/debug_output.h"
#include "stb/stb_image.h"

export module renderer.load_image;

import service.locator;
import sdk.base_filesystem;
import sdk.parser.vtex;

namespace render
{
    struct d3d_texture_destructor_s
    {
        void operator()(ID3D11ShaderResourceView* svr) const
        {
            if (svr) {
                svr->Release();
            }
        }
    };

    struct stb_content_destructor_s
    {
        void operator()(uint8_t* data) const
        {
            if (data) {
                stbi_image_free(data);
            }
        }
    };

    export using d3d_texture_t = std::unique_ptr<ID3D11ShaderResourceView, d3d_texture_destructor_s>;

    using stb_content_t = std::unique_ptr<uint8_t, stb_content_destructor_s>;

    bool CreateD3D11Texture(ID3D11Device* d3dDevice, ID3D11ShaderResourceView** output, const void* rgbaPixels, uint16_t width, uint16_t height)
    {
        if (!rgbaPixels || !d3dDevice) {
            return false;
        }

        D3D11_TEXTURE2D_DESC desc{};
        desc.Width = width;
        desc.Height = height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA subResource;
        subResource.pSysMem = rgbaPixels;
        subResource.SysMemPitch = width * 4;
        subResource.SysMemSlicePitch = 0;

        ID3D11Texture2D* pTexture = nullptr;
        HRESULT hr = d3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);
        if (FAILED(hr))
            return false;

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        ZeroMemory(&srvDesc, sizeof(srvDesc));
        srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = desc.MipLevels;
        srvDesc.Texture2D.MostDetailedMip = 0;

        hr = d3dDevice->CreateShaderResourceView(pTexture, &srvDesc, output);
        pTexture->Release();

        return SUCCEEDED(hr);
    }

    export d3d_texture_t LoadVTEX(ID3D11Device* d3dDevice, const std::string& path)
    {
        std::vector<uint8_t> fileData;
        if (!C_ServiceLocator::Get<sdk::C_BaseFileSystem>()->readFile(path, fileData)) {
            dbg("Unable to read virtual file: {}", path);
            return nullptr;
        }

        const auto vtexResult = sdk::DecodeVtexPNG(fileData);
        if (!vtexResult.has_value()) {
            dbg("Unable to decode vtex png: {}", path);
            return nullptr;
        }

        ID3D11ShaderResourceView* textureView = nullptr;
        if (!CreateD3D11Texture(d3dDevice, &textureView, vtexResult->pixels.data(), vtexResult->width, vtexResult->height)) {
            dbg("Unable to create D3D11 texture from vtex png: {}", path);
            return nullptr;
        }

        return d3d_texture_t{textureView};
    }

    export d3d_texture_t LoadPNG(ID3D11Device* d3dDevice, const std::string& path)
    {
        int32_t originalWidth = 0, originalHeight = 0, channels = 0;
        const stb_content_t imageData(stbi_load(path.c_str(), &originalWidth, &originalHeight, &channels, 4));
        if (!imageData) {
            dbg("Unable to load file: {}", path);
            return nullptr;
        }

        ID3D11ShaderResourceView* textureView = nullptr;
        if (!CreateD3D11Texture(d3dDevice, &textureView, imageData.get(), originalWidth, originalHeight)) {
            dbg("Unable to create D3D11 texture from png: {}", path);
            return nullptr;
        }

        return d3d_texture_t{textureView};
    }
}
