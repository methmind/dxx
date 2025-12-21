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
        DirectX::SimpleMath::Matrix worldToView_;
        DirectX::SimpleMath::Matrix worldToProjection_;

        void onGetMatricesFunc(math::matrix4x4_s* worldToView, math::matrix4x4_s* worldToProjection);

    public:

        [[nodiscard]] const DirectX::SimpleMath::Matrix& getWorldToView() const { return this->worldToView_; }

        [[nodiscard]] const DirectX::SimpleMath::Matrix& getWorldToProjection() const { return this->worldToProjection_; }

        bool initialize();

        C_MatricesSystem() = default;

        ~C_MatricesSystem() = default;
    };
} // sdk

#endif //DXX_DLC_SDK_MATRICES_SYSTEM_H