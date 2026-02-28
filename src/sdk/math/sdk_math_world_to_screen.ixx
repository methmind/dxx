//
// Created by sexey on 28.02.2026.
//
module;

export module sdk.math.world_to_screen;

import sdk.math.matrix;
import sdk.math.vector;

export namespace sdk
{
    bool WorldToScreen(const vector3_t& vecOrigin, const vector2_t& screenSize, const matrix_t& worldToProjectionMatrix, vector2_t& output)
    {
        const vector4_t clip = vector4_t::Transform(vector4_t(vecOrigin.x, vecOrigin.y, vecOrigin.z, 1.0f), worldToProjectionMatrix);
        if (clip.w < 0.001f) {
            return false;
        }

        const float invW = 1.0f / clip.w;
        const float x = clip.x * invW;
        const float y = clip.y * invW;

        const float cX = screenSize.x * 0.5f;
        const float cY = screenSize.y * 0.5f;
        output.x = cX * (1.0f + x);
        output.y = cY * (1.0f - y);

        return true;
    }
}