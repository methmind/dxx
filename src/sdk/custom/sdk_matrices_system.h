//
// Created by sexey on 18.12.2025.
//

#ifndef DXX_DLC_SDK_MATRICES_SYSTEM_H
#define DXX_DLC_SDK_MATRICES_SYSTEM_H

#include "sdk/math/sdk_math_matrix.h"
#include "SimpleMath.h"

namespace sdk::custom
{
    class C_MatricesSystem
    {
    private:
        DirectX::SimpleMath::Matrix viewMatrix_;
        DirectX::SimpleMath::Matrix worldProjectionMatrix_;
        DirectX::SimpleMath::Matrix worldPixelMatrix_;

        void onGetMatricesFunc(math::matrix4x4_s* viewWorld, math::matrix4x4_s* worldProjection, math::matrix4x4_s* worldPixel);

    public:

        [[nodiscard]] const DirectX::SimpleMath::Matrix& getViewMatrix() const { return this->viewMatrix_; }

        [[nodiscard]] const DirectX::SimpleMath::Matrix& getWorldProjectionMatrix() const { return this->worldProjectionMatrix_; }

        [[nodiscard]] const DirectX::SimpleMath::Matrix& getWorldPixelMatrix() const { return this->worldPixelMatrix_; }

        bool initialize();

        C_MatricesSystem() = default;

        ~C_MatricesSystem() = default;
    };
} // sdk

#endif //DXX_DLC_SDK_MATRICES_SYSTEM_H