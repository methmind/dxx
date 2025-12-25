//
// Created by sexey on 18.12.2025.
//

#include "feature_virtual_camera.h"

#include "debug/debug_output.h"
#include "sdk/singleton/sdk_dota_camera_manager.h"
#include "renderer/renderer_math.h"

namespace feature
{
    void C_FeatureVirtualCamera::UpdateCameraMatrix(DirectX::SimpleMath::Matrix& target,
        const DirectX::SimpleMath::Matrix& source, const DirectX::SimpleMath::Vector3& newPosition, const float newDistance,
        const DirectX::SimpleMath::Vector3& oldPosition, const float oldDistance)
    {
        target = source;

        const DirectX::SimpleMath::Vector3 diffPosition = newPosition - oldPosition;
        const float diffDistance = newDistance - oldDistance;

        // Эпсилон для игнорирования шума
        constexpr float EPSILON_SQ = 1e-5f;
        if (diffPosition.LengthSquared() > EPSILON_SQ)
        {
            // Это эквивалентно: diff.x * Col1 + diff.y * Col2 + diff.z * Col3 (для первых 3 строк)
            const auto deltaTranslation = DirectX::SimpleMath::Vector3::TransformNormal(diffPosition, source);

            // Нам нужно скалярное произведение diffPosition на 4-й столбец матрицы (оси проекции).
            const DirectX::SimpleMath::Vector3 column4(source._14, source._24, source._34);
            const float deltaW = diffPosition.Dot(column4);

            // Вычитаем дельту из трансляции (инвертируем движение мира относительно камеры)
            target._41 -= deltaTranslation.x;
            target._42 -= deltaTranslation.y;
            target._43 -= deltaTranslation.z;
            target._44 -= deltaW;
        }

        // Source 2 изменяет дистанцию специфично: игнорируя ось Y, но учитывая "перекос" проекции в X и Z.
        if (std::abs(diffDistance) > 1e-4f && source._34 != 0.f)
        {
            // _34 отвечает за перспективное искажение по Z (обычно -1 или близко к нему в ViewProj)
            const float projectionScale = diffDistance / source._34;

            // Корректируем X пропорционально "повороту" матрицы (_31)
            target._41 += source._31 * projectionScale;

            // Y (_42) НЕ ТРОГАЕМ. В Dota 2 зум не смещает центр экрана по вертикали.

            // Корректируем Z пропорционально направлению взгляда (_33)
            target._43 += source._33 * projectionScale;

            // Линейное изменение проекционного смещения W
            target._44 += diffDistance;
        }
    }

    DirectX::SimpleMath::Vector2 C_FeatureVirtualCamera::CalculateLookAt(const DirectX::SimpleMath::Vector3& position,
        const float distance, const float pitch)
    {
        const float pitchRad = DirectX::XMConvertToRadians(pitch);
        const float horizontalDist = distance * std::cos(pitchRad);

        return { position.x, position.y - horizontalDist };
    }

    void C_FeatureVirtualCamera::recalculateMatrix()
    {
        UpdateCameraMatrix(this->virtualWorldToProjectionMatrix_, this->matricesSystem_->getWorldToProjection(),
            reinterpret_cast<const DirectX::SimpleMath::Vector3&>(this->virtualCamera_.getCameraPosition()),
            this->virtualCamera_.getCameraDistance(),
            reinterpret_cast<const DirectX::SimpleMath::Vector3&>(this->originalCamera_->getCameraPosition()),
            this->originalCamera_->getCameraDistance()
        );
    }

    bool C_FeatureVirtualCamera::initialize()
    {
        this->originalCamera_ = C_ServiceLocator::getInstance<sdk::singleton::C_DotaCamaraManager>()->getPrimaryCamera();
        if (!this->originalCamera_) {
            dbg("sdk::singleton::C_DotaCamaraManager::getPrimaryCamera == nullptr!");
            return false;
        }

        const auto& originalCameraPosition = this->originalCamera_->getCameraPosition();
        if (this->originalCamera_->getCameraDistance() < 100.0f || std::fabs(originalCameraPosition.z) < 1.0f) {
            return false;
        }

        updateCameraPosition(originalCameraPosition);
        this->isInitialized_ = true;

        return true;
    }

    DirectX::SimpleMath::Vector2 C_FeatureVirtualCamera::getLookAt() const
    {
        return CalculateLookAt(reinterpret_cast<DirectX::SimpleMath::Vector3&>(this->virtualCamera_.getCameraPosition()),
            this->virtualCamera_.getCameraDistance(),
            this->virtualCamera_.getViewAngles().x
        );
    }

    void C_FeatureVirtualCamera::updateCameraPosition(const sdk::math::vector3& position)
    {
        this->virtualCamera_ = *this->originalCamera_; //todo We truly should reverse traceray...
        this->virtualCamera_.getCameraDistance() = DEFAULT_CAMERA_DISTANCE;
        this->virtualCamera_.setCameraPosition(position);

        recalculateMatrix();
    }
} // feature