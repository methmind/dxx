//
// Created by sexey on 19.12.2025.
//

#include "renderer_math.h"

using namespace DirectX::SimpleMath;

namespace renderer
{
    void AdjustMatrix(Matrix& target, const Matrix& source, const Vector3& targetPosition, float targetDistance,
        const Vector3& sourcePosition, float sourceDistance)
    {
        target = source;
        // 2. Считаем дельты
        Vector3 dPos = targetPosition - sourcePosition;
        float dDist = targetDistance - sourceDistance;

        // 3. Вычисляем проекцию смещения через саму матрицу.
        // Это заменяет твой цикл вычисления m_dpos.
        // TransformNormal вращает и масштабирует вектор, но НЕ применяет трансляцию (W=0).
        // Это ровно то, что делал твой код: source->m[row][0]*x + ...
        Vector4 dPosProjected = Vector4::Transform(Vector4(dPos.x, dPos.y, dPos.z, 0.0f), source);

        // 4. Вычитаем проекцию из столбца трансляции.
        // В SimpleMath (DXM Transposed) трансляция лежит в 4-й строке (_41, _42, _43, _44).
        // Но твой код работал с "сырыми" данными Source 2, где вектора в строках, а трансляция в 4-м столбце.

        // ВНИМАНИЕ: Если ты передаешь сюда Matrix из SimpleMath (который уже транспонирован при чтении),
        // то доступ должен быть как к строкам _41.._44.

        // T_new = T_old - ProjectedDelta
        target._41 -= dPosProjected.x;
        target._42 -= dPosProjected.y;
        target._43 -= dPosProjected.z;
        target._44 -= dPosProjected.w;

        // 5. Коррекция дистанции (ddist)
        // В твоем коде это добавлялось к row 2 и row 3 (индексы).
        // В транспонированной матрице (SimpleMath) это соответствует столбцам _3 и _4.
        // То есть элементам _33, _43 (для Z) и _34, _44 (для W).

        // Твой код: target.m[row][3] = ... + k_ddist[row]
        // k_ddist был {0, 0, ddist, ddist}.
        // Это значит мы добавляем ddist к 3-му и 4-му элементу СТОЛБЦА трансляции.

        // В DXM (строка трансляции _41.._44):
        // Мы добавляем к _43 (Z) и _44 (W).

        target._43 += dDist;
        target._44 += dDist;
    }

    bool WorldToScreen_Pixel(const Vector3& worldPosition, Vector2& screenPosition, const Matrix& worldPixelMatrix)
    {
        const auto vecOrigin4 = Vector4(worldPosition.x, worldPosition.y, worldPosition.z, 1.0f);

        const auto clip = Vector4::Transform(vecOrigin4, worldPixelMatrix);
        if (clip.w < 0.001f) {
            return false;
        }

        screenPosition.x = clip.x / clip.w;
        screenPosition.y = clip.y / clip.w;
        return true;
    }
} // renderer