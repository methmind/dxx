//
// Created by sexey on 18.12.2025.
//

#ifndef DXX_DLC_FEATURE_VIRTUAL_CAMERA_H
#define DXX_DLC_FEATURE_VIRTUAL_CAMERA_H

#include "sdk/custom/sdk_matrices_system.h"
#include "sdk/datatype/sdk_base_model_entity.h"
#include "sdk/datatype/sdk_dota_camera.h"

namespace feature
{
    class C_FeatureVirtualCamera
    {
    private:
        sdk::datatype::C_DotaCamera virtualCamera_;
        DirectX::SimpleMath::Matrix virtualWorldPixelMatrix_;
        std::shared_ptr<sdk::custom::C_MatricesSystem> matricesSystem_;

        static void UpdateCameraMatrix(DirectX::SimpleMath::Matrix& target, const DirectX::SimpleMath::Matrix& source,
            const DirectX::SimpleMath::Vector3& newPosition, float newDistance,
            const DirectX::SimpleMath::Vector3& oldPosition, float oldDistance
        );

        [[nodiscard]] static DirectX::SimpleMath::Vector2 CalculateLookAt(const DirectX::SimpleMath::Vector3& position,
            float distance, float pitch
        );

        __forceinline bool recalculateMatrix();

    public:

        [[nodiscard]] const DirectX::SimpleMath::Matrix& getWorldToViewMatrix() const { return this->matricesSystem_->getWorldToView(); }

        [[nodiscard]] const DirectX::SimpleMath::Matrix& getWorldToProjectionMatrix() const { return this->virtualWorldPixelMatrix_; }

        [[nodiscard]] const sdk::math::vector3& getCameraPosition() const { return this->virtualCamera_.getCameraPosition(); }

        [[nodiscard]] float getCameraDistance() const { return this->virtualCamera_.getCameraDistance(); }

        [[nodiscard]] DirectX::SimpleMath::Vector2 getLookAt() const;

        bool updateCameraPosition(const sdk::math::vector3& position);

        bool initialize();

        explicit C_FeatureVirtualCamera(const std::shared_ptr<sdk::custom::C_MatricesSystem>& matricesSystem) : matricesSystem_(matricesSystem) {}

        ~C_FeatureVirtualCamera() = default;
    };
} // feature

#endif //DXX_DLC_FEATURE_VIRTUAL_CAMERA_H