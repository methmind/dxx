//
// Created by sexey on 19.12.2025.
//

#ifndef DXX_DLC_RENDERER_MATH_H
#define DXX_DLC_RENDERER_MATH_H

#include "SimpleMath.h"

namespace renderer
{
    void AdjustMatrix(DirectX::SimpleMath::Matrix& target, const DirectX::SimpleMath::Matrix& source,
        const DirectX::SimpleMath::Vector3& targetPosition, float targetDistance,
        const DirectX::SimpleMath::Vector3& sourcePosition, float sourceDistance
    );

    bool WorldToScreen_Pixel(const DirectX::SimpleMath::Vector3& worldPosition, DirectX::SimpleMath::Vector2& screenPosition,
        const DirectX::SimpleMath::Matrix& worldPixelMatrix
    );
} // renderer

#endif //DXX_DLC_RENDERER_MATH_H