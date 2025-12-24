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
    constexpr auto DEFAULT_CAMERA_DISTANCE = 1200.0f;

    class C_FeatureVirtualCamera
    {
    private:
        bool isInitialized_;
        sdk::datatype::C_DotaCamera virtualCamera_;
        DirectX::SimpleMath::Matrix virtualWorldToProjectionMatrix_;
        std::shared_ptr<sdk::custom::C_MatricesSystem> matricesSystem_;
        sdk::datatype::C_DotaCamera* originalCamera_{};

        static void UpdateCameraMatrix(DirectX::SimpleMath::Matrix& target, const DirectX::SimpleMath::Matrix& source,
            const DirectX::SimpleMath::Vector3& newPosition, float newDistance,
            const DirectX::SimpleMath::Vector3& oldPosition, float oldDistance
        );

        [[nodiscard]] static DirectX::SimpleMath::Vector2 CalculateLookAt(const DirectX::SimpleMath::Vector3& position,
            float distance, float pitch
        );

        __forceinline void recalculateMatrix();

    public:

        [[nodiscard]] const DirectX::SimpleMath::Matrix& getWorldToViewMatrix() const { return this->matricesSystem_->getWorldToView(); }

        [[nodiscard]] const DirectX::SimpleMath::Matrix& getWorldToProjectionMatrix() const { return this->virtualWorldToProjectionMatrix_; }

        [[nodiscard]] const sdk::math::vector3& getCameraPosition() const { return this->virtualCamera_.getCameraPosition(); }

        [[nodiscard]] float getCameraDistance() const { return this->virtualCamera_.getCameraDistance(); }

        [[nodiscard]] DirectX::SimpleMath::Vector2 getLookAt() const;

        void updateCameraPosition(const sdk::math::vector3& position);

        bool initialize();

        bool isInitialized() const { return this->isInitialized_; }

        void dispose() { this->isInitialized_ = false; }

        explicit C_FeatureVirtualCamera(const std::shared_ptr<sdk::custom::C_MatricesSystem>& matricesSystem) :
           isInitialized_(false), matricesSystem_(matricesSystem) {}

        ~C_FeatureVirtualCamera() = default;
    };
} // feature

#endif //DXX_DLC_FEATURE_VIRTUAL_CAMERA_H