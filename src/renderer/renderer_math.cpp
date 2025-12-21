//
// Created by sexey on 19.12.2025.
//

#include "renderer_math.h"

using namespace DirectX::SimpleMath;

namespace render
{
    bool WorldToScreen(const Vector3& vecOrigin, Vector2& vecScreen, const Vector2& screenSize, const Matrix& worldToProjectionMatrix)
    {
        const Vector4 clip = Vector4::Transform(Vector4(vecOrigin.x, vecOrigin.y, vecOrigin.z, 1.0f), worldToProjectionMatrix);
        if (clip.w < 0.001f) {
            return false;
        }

        const float invW = 1.0f / clip.w;
        const float x = clip.x * invW;
        const float y = clip.y * invW;

        const float cX = screenSize.x * 0.5f;
        const float cY = screenSize.y * 0.5f;
        vecScreen.x = cX * (1.0f + x);
        vecScreen.y = cY * (1.0f - y);

        return true;
    }
} // renderer