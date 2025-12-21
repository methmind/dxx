//
// Created by sexey on 19.12.2025.
//

#ifndef DXX_DLC_RENDERER_MATH_H
#define DXX_DLC_RENDERER_MATH_H

#include "SimpleMath.h"

namespace render
{
    bool WorldToScreen(const DirectX::SimpleMath::Vector3& vecOrigin, DirectX::SimpleMath::Vector2& vecScreen,
        const DirectX::SimpleMath::Vector2& screenSize,
        const DirectX::SimpleMath::Matrix& worldToProjectionMatrix
    );
} // renderer

#endif //DXX_DLC_RENDERER_MATH_H