//
// Created by sexey on 10.12.2025.
//

#include "feature_virtual_camera_manager.h"

#include "imgui_internal.h"
#include "hook/hook_dispatcher.h"
#include "hook/impl/hook_impl_type.h"
#include "memory/pattern_scanner.h"
#include "sdk/sdk_signature.h"
#include "sdk/custom/sdk_entity_list.h"
#include "sdk/math/sdk_math_vector3.h"
#include "sdk/singleton/sdk_base_game_system_factory.h"
#include "sdk/singleton/sdk_render_game_system.h"
#include "sdk/math/sdk_math_quantize_point.h"
#include "service_locator/service_locator.h"
#include "sdk/memory/sdk_memory.h"
#include "sdk/singleton/sdk_dota_camera_manager.h"
#include "sdk/singleton/sdk_source2_engine_to_client.h"

#include "renderer/renderer_math.h"

namespace feature
{
    bool C_FeatureVirtualCameraManager::initializeCamera()
    {
        if (this->isInitialized_) {
            return true;
        }

        if (!this->serverCamera_.initialize()) {
            return false;
        }

        this->isInitialized_ = true;
        return true;
    }

    bool WorldToScreenPixel(const DirectX::SimpleMath::Vector3& vecOrigin, DirectX::SimpleMath::Vector2& vecScreen, const DirectX::SimpleMath::Matrix& pixelMatrix)
    {
        // W-компонент
        float w = pixelMatrix.m[3][0] * vecOrigin.x +
                  pixelMatrix.m[3][1] * vecOrigin.y +
                  pixelMatrix.m[3][2] * vecOrigin.z +
                  pixelMatrix.m[3][3];

        if (w < 0.001f) return false;

        float invW = 1.0f / w;

        // Координаты сразу в пикселях!
        vecScreen.x = (pixelMatrix.m[0][0] * vecOrigin.x +
                       pixelMatrix.m[0][1] * vecOrigin.y +
                       pixelMatrix.m[0][2] * vecOrigin.z +
                       pixelMatrix.m[0][3]) * invW;

        vecScreen.y = (pixelMatrix.m[1][0] * vecOrigin.x +
                       pixelMatrix.m[1][1] * vecOrigin.y +
                       pixelMatrix.m[1][2] * vecOrigin.z +
                       pixelMatrix.m[1][3]) * invW;

        return true;
    }

    bool WorldToScreen(const DirectX::SimpleMath::Vector3& vecOrigin, DirectX::SimpleMath::Vector2& vecScreen, const DirectX::SimpleMath::Matrix& viewMatrix)
    {
        // 1. Считаем W-компонент (глубину для проекции)
        const float flWidth = viewMatrix.m[3][0] * vecOrigin.x +
                              viewMatrix.m[3][1] * vecOrigin.y +
                              viewMatrix.m[3][2] * vecOrigin.z +
                              viewMatrix.m[3][3];

        // Проверка, что точка не за камерой (или слишком близко)
        if (flWidth < 0.001f)
            return false;

        // 2. Считаем инверсию для деления
        const float flInverse = 1.0f / flWidth;

        // 3. Проецируем координаты (умножаем X/Y вектора на матрицу и делим на W)
        vecScreen.x = (viewMatrix.m[0][0] * vecOrigin.x +
                       viewMatrix.m[0][1] * vecOrigin.y +
                       viewMatrix.m[0][2] * vecOrigin.z +
                       viewMatrix.m[0][3]) * flInverse;

        vecScreen.y = (viewMatrix.m[1][0] * vecOrigin.x +
                       viewMatrix.m[1][1] * vecOrigin.y +
                       viewMatrix.m[1][2] * vecOrigin.z +
                       viewMatrix.m[1][3]) * flInverse;

        // 4. Переводим из NDC (-1..1) в координаты экрана (пиксели)
        const ImVec2 vecDisplaySize = ImGui::GetIO().DisplaySize;

        vecScreen.x = (vecDisplaySize.x * 0.5f) + (vecScreen.x * vecDisplaySize.x) * 0.5f;
        vecScreen.y = (vecDisplaySize.y * 0.5f) - (vecScreen.y * vecDisplaySize.y) * 0.5f;

        return true;
    }

    void C_FeatureVirtualCameraManager::onCreateMove(sdk::datatype::user_cmd_s* userCmd)
    {
        if (!initializeCamera()) {
            return;
        }

        auto& cmd = userCmd->cmd;
        DirectX::SimpleMath::Vector3 crosshairTrace = {cmd.crosshairtrace().x(),cmd.crosshairtrace().y(), cmd.crosshairtrace().z()};

        DirectX::SimpleMath::Vector2 screenPos;
        bool isVisible = WorldToScreen(crosshairTrace, screenPos, this->serverCamera_.getWorldPixelMatrix());

        // 2. Рассчитываем смещение, если точка выходит за рамки виртуального экрана
        sdk::math::vector3 worldDiff = {0, 0, 0};

        // Параметры "виртуального экрана" (обычно совпадают с реальным)
        const auto displaySize = C_ServiceLocator::getInstance<sdk::singleton::C_Source2EngineToClient>()->getScreenSize();
        const float padding = 50.0f; // Отступ, чтобы не держать курсор на самом краю

        float deltaX = 0.0f;
        float deltaY = 0.0f;

        if (!isVisible) {
            // Если точка за спиной виртуальной камеры или слишком далеко
            // Просто переносим виртуальную камеру к точке (сохраняя Z и дистанцию)
            auto camPos = this->serverCamera_.getCameraPosition();
            worldDiff.x = crosshairTrace.x - camPos.x;
            worldDiff.y = crosshairTrace.y - camPos.y;
        } else {
            // Точка "на экране", проверяем границы
            if (screenPos.x > displaySize.x - padding) {
                deltaX = screenPos.x - (displaySize.x - padding);
            } else if (screenPos.x < padding) {
                deltaX = screenPos.x - padding;
            }

            if (screenPos.y > displaySize.y - padding) {
                deltaY = screenPos.y - (displaySize.y - padding);
            } else if (screenPos.y < padding) {
                deltaY = screenPos.y - padding;

            }
        }

        // 3. Конвертация Pixel Delta -> World Delta
        if (deltaX != 0.0f || deltaY != 0.0f) {
            // Берем векторы осей из НАСТОЯЩЕЙ матрицы View, так как наклон камеры не меняется
            auto& viewMatrix = this->serverCamera_.getViewMatrix();

            // Вектора (Row Major для Source 2):
            // Right Vector (X камеры)
            float vRightX = viewMatrix.m[0][0];
            float vRightY = viewMatrix.m[0][1];

            // Up Vector (Y камеры на экране проецируется на этот вектор в мире)
            float vUpX = viewMatrix.m[1][0];
            float vUpY = viewMatrix.m[1][1];

            // Масштаб (Units per Pixel).
            // 1200 distance ~ 1.2 units/pixel.
            float dist = this->serverCamera_.getCameraDistance();
            float scale = dist * 0.001f;

            // Считаем смещение
            // Если точка ушла вправо (deltaX > 0), камеру надо сдвинуть вправо (вдоль vRight)
            // Если точка ушла вниз (deltaY > 0), камеру надо сдвинуть "вниз" (против vUp, т.к. Y экрана растет вниз)

            worldDiff.x = (vRightX * deltaX - vUpX * deltaY) * scale;
            worldDiff.y = (vRightY * deltaX - vUpY * deltaY) * scale;
        }

        // 4. Применяем смещение
        if (worldDiff.x != 0 || worldDiff.y != 0) {
            auto newPos = this->serverCamera_.getCameraPosition();
            newPos.x += worldDiff.x;
            newPos.y += worldDiff.y;

            // Обновляем виртуальную камеру и пересчитываем её матрицу
            this->serverCamera_.updateCameraPosition(newPos);
        }


        DirectX::SimpleMath::Vector2 view, view2;
        WorldToScreen(crosshairTrace, view, this->serverCamera_.getWorldPixelMatrix());
        WorldToScreen(crosshairTrace, view2, this->matricesSystem_->getWorldProjectionMatrix());

        this->render_->getPrimitivesRenderFrame()->enqueue([view, view2](ImDrawList* drawList) {
            drawList->AddCircle(ImVec2(view.x, view.y), 25, IM_COL32(255, 0, 0, 255), 10, 5);
            drawList->AddCircle(ImVec2(view2.x, view2.y), 25, IM_COL32(255, 0, 255, 255), 10, 5);
        });
        this->render_->getPrimitivesRenderFrame()->bake();

        auto quantized = sdk::math::QuantizePoint(displaySize, reinterpret_cast<sdk::math::vector2&>(view));
        auto lookat = this->serverCamera_.getLookAtPosition();
        //C_ServiceLocator::getInstance<sdk::singleton::C_DotaCamaraManager>()->getPrimaryCamera()->setCameraPosition(this->serverCamera_.getCameraPosition());

        cmd.set_cameraposition_x(lookat.x);
        cmd.set_cameraposition_y(lookat.y);
        cmd.mutable_base()->set_mousedx(quantized.x);
        cmd.mutable_base()->set_mousedy(quantized.y);
    }

    bool C_FeatureVirtualCameraManager::initialize()
    {
        const auto hookDispatcher = C_ServiceLocator::getInstance<hook::C_HookDispatcher>();

        hookDispatcher->subscribe<sdk::datatype::user_cmd_s*>(
            static_cast<hook::hook_id_t>(hook::impl::hook_impl_type_e::CREATE_MOVE),
            [this](sdk::datatype::user_cmd_s* cmd){
                onCreateMove(cmd);
            }
        );

        hookDispatcher->subscribe(
            static_cast<hook::hook_id_t>(hook::impl::hook_impl_type_e::ON_LEVEL_INIT),
            [this]{ onLevelInit(); }
        );

        return true;
    }
} // feature