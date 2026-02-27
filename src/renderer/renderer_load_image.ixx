//
// Created by sexey on 26.02.2026.
//
module;
#include <d3d11.h>

#include "debug/debug_output.h"

export module renderer.load_image;

import service.locator;
import sdk.base_filesystem;
import sdk.parser.vtex;
import sdk.panorama.ui_engine;
import sdk.panorama.image_resource_manager;

namespace render
{
    /*
     * @brief Загружает текстуру из файла, используя ресурсный менеджер Source 2 UI.
     * Пути могут быть:
     * - file://
     * - http://
     * - https://
     * - s2r://
     * - raw://
     * - panel://
     * - panel-background://
     * Например: s2r://panorama/images/heroes/npc_dota_hero_axe_png.vtex
     */
    export ID3D11ShaderResourceView* LoadTexture(const std::string& path)
    {
        const auto uiEngine = C_ServiceLocator::Get<sdk::C_PanoramaUIEngine>()->getUIEngineSource2();
        if (!uiEngine) {
            dbg("Failed to get Source 2 UI Engine");
            return nullptr;
        }

        const auto resourceManager = uiEngine->getImageResourceManager();
        if (!resourceManager) {
            dbg("Failed to get Image Resource Manager");
            return nullptr;
        }

        const auto imageProxy = resourceManager->loadImage(path.c_str(), sdk::image_format_e::RGBA8888);
        if (!imageProxy) {
            dbg("Failed to load image proxy for path: {}", path);
            return nullptr;
        }

        const auto textureView = imageProxy->getSharedResourceView();
        if (!textureView) {
            dbg("Failed to get shared resource view for image proxy with path: {}", path);
            return nullptr;
        }

        return textureView;
    }
}
