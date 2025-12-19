//
// Created by sexey on 18.12.2025.
//

#include "feature_virtual_camera.h"

#include "debug/debug_output.h"
#include "sdk/singleton/sdk_dota_camera_manager.h"
#include "renderer/renderer_math.h"

namespace feature
{

    void AdjustCameraMatrix(DirectX::SimpleMath::Matrix& target, const DirectX::SimpleMath::Matrix& source,
                       const sdk::math::vector3& oldPos, float oldDist,
                       const sdk::math::vector3& newPos, float newDist) {
        // 1. Копируем неизменяемые первые 3 столбца (M: ориентация + проекция)
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 3; ++col) {
                target.m[row][col] = source.m[row][col];
            }
        }

        // 2. Δpos = newPos - oldPos
        sdk::math::vector3 dpos = {
            newPos.x - oldPos.x,
            newPos.y - oldPos.y,
            newPos.z - oldPos.z
        };

        // 3. Вычисляем M · Δpos (4-мерный вектор)
        float m_dpos[4] = {0.0f};
        for (int row = 0; row < 4; ++row) {
            m_dpos[row] = source.m[row][0] * dpos.x +
                          source.m[row][1] * dpos.y +
                          source.m[row][2] * dpos.z;
        }

        // 4. T_pos = T_source - M · Δpos
        float t_pos[4];
        for (int row = 0; row < 4; ++row) {
            t_pos[row] = source.m[row][3] - m_dpos[row];
        }

        // 5. Δdist = newDist - oldDist, K · Δdist ≈ [0, 0, Δdist, Δdist]
        // (Точная K из дампа: z=1.00146, но 1.0f достаточно для float precision)
        float ddist = newDist - oldDist;
        float k_ddist[4] = {0.0f, 0.0f, ddist, ddist};

        // 6. Финальный 4-й столбец: T_pos + K · Δdist
        for (int row = 0; row < 4; ++row) {
            target.m[row][3] = t_pos[row] + k_ddist[row];
        }
    }

    bool C_FeatureVirtualCamera::recalculateMatrix()
    {
        const auto originalCamera = C_ServiceLocator::getInstance<sdk::singleton::C_DotaCamaraManager>()->getPrimaryCamera();
        if (!originalCamera) {
            dbg("sdk::singleton::C_DotaCamaraManager::getPrimaryCamera == nullptr!");
            return false;
        }

        AdjustCameraMatrix(this->virtualWorldPixelMatrix_, this->matricesSystem_->getWorldProjectionMatrix(),
           this->virtualCamera_.getCameraPosition(),
           this->virtualCamera_.getCameraDistance(),
           originalCamera->getCameraPosition(),
           originalCamera->getCameraDistance()
        );

        /*renderer::AdjustMatrix(this->virtualWorldPixelMatrix_, this->matricesSystem_->getWorldPixelMatrix(),
           reinterpret_cast<const DirectX::SimpleMath::Vector3&>(this->virtualCamera_.getCameraPosition()),
           this->virtualCamera_.getCameraDistance(),
           reinterpret_cast<const DirectX::SimpleMath::Vector3&>(originalCamera->getCameraPosition()),
           originalCamera->getCameraDistance()
        );*/

        return true;
    }

    bool C_FeatureVirtualCamera::initialize()
    {
        const auto originalCamera = C_ServiceLocator::getInstance<sdk::singleton::C_DotaCamaraManager>()->getPrimaryCamera();
        if (!originalCamera) {
            dbg("sdk::singleton::C_DotaCamaraManager::getPrimaryCamera == nullptr!");
            return false;
        }

        const auto originalCameraPosition = originalCamera->getCameraPosition();
        if (originalCamera->getCameraDistance() < 100.0f || std::fabs(originalCameraPosition.z) < 1.0f) {
            return false;
        }

        this->virtualCamera_ = *originalCamera;
        this->virtualCamera_.getCameraDistance() = 1200;
        if (!updateCameraPosition(originalCameraPosition)) {
            dbg("Unable to update virtual camera position!");
            return false;
        }

        return true;
    }

    bool C_FeatureVirtualCamera::updateCameraPosition(const sdk::math::vector3& position)
    {
        this->virtualCamera_.setCameraPosition(position);
        return recalculateMatrix();
    }
} // feature