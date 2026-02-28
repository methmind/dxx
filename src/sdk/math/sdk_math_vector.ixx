//
// Created by sexey on 28.02.2026.
//
module;
#include "SimpleMath.h"

export module sdk.math.vector;

export namespace sdk
{
    /*
     * @note Вообще нам откровенно "везет", что в Source 2 используются совпадающие математические структуры с DirectX.
     */

    using vector2_t = DirectX::SimpleMath::Vector2;

    using vector3_t = DirectX::SimpleMath::Vector3;

    using vector4_t = DirectX::SimpleMath::Vector4;
}