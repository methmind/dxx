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

        __forceinline bool recalculateMatrix();

    public:

        [[nodiscard]] const DirectX::SimpleMath::Matrix& getViewMatrix() const { return this->matricesSystem_->getViewMatrix(); }

        [[nodiscard]] const DirectX::SimpleMath::Matrix& getWorldPixelMatrix() const { return this->virtualWorldPixelMatrix_; }

        [[nodiscard]] const sdk::math::vector3& getCameraPosition() const { return this->virtualCamera_.getCameraPosition(); }

        [[nodiscard]] const sdk::math::vector2& getLookAtPosition() const { return this->virtualCamera_.getLookAt(); }

        [[nodiscard]] float getCameraDistance() const { return this->virtualCamera_.getCameraDistance(); }

        bool updateCameraPosition(const sdk::math::vector3& position);

        bool initialize();

        explicit C_FeatureVirtualCamera(const std::shared_ptr<sdk::custom::C_MatricesSystem>& matricesSystem) : matricesSystem_(matricesSystem) {}

        ~C_FeatureVirtualCamera() = default;
    };
} // feature

#endif //DXX_DLC_FEATURE_VIRTUAL_CAMERA_H