//
// Created by sexey on 10.12.2025.
//

#include "feature_virtual_camera.h"

#include "imgui_internal.h"
#include "hook/hook_dispatcher.h"
#include "hook/impl/hook_impl_type.h"
#include "memory/pattern_scanner.h"
#include "sdk/sdk_signature.h"
#include "sdk/math/sdk_math_vector3.h"
#include "sdk/singleton/sdk_base_game_system_factory.h"
#include "sdk/singleton/sdk_render_game_system.h"
#include "sdk/math/sdk_math_quantize_point.h"
#include "service_locator/service_locator.h"

#include "directx/Inc/DirectXMath.h"
#include "sdk/memory/sdk_memory.h"
#include "sdk/singleton/sdk_dota_camera_manager.h"

using namespace DirectX;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define DEG2RAD(x) ((float)(x) * (float)(M_PI / 180.0f))

namespace feature
{
    struct Matrix4x4 {
        float m[4][4];
    };

    void C_FeatureCameraEmulator::fixCameraPosition(sdk::datatype::user_cmd_s* userCmd)
    {
        const float flCurrentDist = 2000.0f; // Текущая (читерская) дистанция
        const float flTargetDist = 1200.0f;  // Целевая (легитимная) дистанция
        const float flDiff = flCurrentDist - flTargetDist; // 800 юнитов, на которые нужно сдвинуть камеру вперед

        // 2. Получаем углы обзора камеры из userCmd
        // В usercmd.proto: viewangles находятся внутри сообщения base (CBaseUserCmdPB)
        // Структура likely: userCmd->base.viewangles
        float pitch = userCmd->cmd.base().viewangles().x();
        float yaw = userCmd->cmd.base().viewangles().y();

        // 3. Вычисляем Forward вектор камеры
        // Нам нужна проекция вектора взгляда на плоскость XY для корректировки позиции x и y.
        // Формула для Forward вектора из углов Эйлера (Pitch/Yaw):
        // x = cos(yaw) * cos(pitch)
        // y = sin(yaw) * cos(pitch)

        float flPitchRad = DEG2RAD(pitch);
        float flYawRad = DEG2RAD(yaw);

        // Вычисляем косинус питча, так как камера смотрит под углом вниз
        float flCosPitch = cos(flPitchRad);

        // Вектор направления камеры на плоскости XY (нормализованный по длине проекции)
        float flForwardX = cos(flYawRad) * flCosPitch;
        float flForwardY = sin(flYawRad) * flCosPitch;

        // 4. Рассчитываем смещение
        // Мы находимся "дальше" (2000), хотим быть "ближе" (1200).
        // Значит, нужно двигаться ВПЕРЕД по направлению взгляда.
        // Смещение = Разница дистанций (гипотенуза) * Вектор направления
        // Однако "дистанция" в Dota 2 (dota_camera_distance) - это расстояние от точки фокуса до глаза.
        // Поэтому мы просто прибавляем вектор разницы.

        // Важный момент: если dota_camera_distance - это прямая дистанция (гипотенуза),
        // то проекция смещения на землю уже учтена через cos(pitch) в формуле вектора.
        // Если же 2000 - это высота, расчет был бы другим, но стандартная команда оперирует дистанцией по лучу.

        float flOffsetX = flDiff * flForwardX;
        float flOffsetY = flDiff * flForwardY;

        // 5. Применяем смещение к текущим координатам
        // Координаты в userCmd (int32) соответствуют позиции при дистанции 2000.
        // Сдвигаем их вперед к точке фокуса на 800 юнитов.
        userCmd->cmd.set_cameraposition_x(userCmd->cmd.cameraposition_x() + (int32_t)flOffsetX);
        userCmd->cmd.set_cameraposition_y(userCmd->cmd.cameraposition_y() + (int32_t)flOffsetY);
    }

    void C_FeatureCameraEmulator::onLevelInit()
    {
        if (!this->dummyCamera_.has_value()) {
            this->dummyCamera_.emplace();
        }

        memcpy(&this->dummyCamera_.value(),
            C_ServiceLocator::getInstance<sdk::singleton::C_DotaCamaraManager>()->getPrimaryCamera(),
            sizeof(sdk::datatype::C_DotaCamera)
        );
    }

    void ConvertCameraDistance(Matrix4x4& matrix, float currentDist, float targetDist) {
        // Вычисляем коэффициент.
        // Если мы уменьшаем дистанцию (2000 -> 1200), объекты должны стать больше,
        // значит координаты на экране должны "разъехаться" от центра -> factor > 1.
        float flScaleFactor = currentDist / targetDist;

        // Масштабируем строку 0 (X-axis в Screen Space)
        matrix.m[0][0] *= flScaleFactor;
        matrix.m[0][1] *= flScaleFactor;
        matrix.m[0][2] *= flScaleFactor;
        matrix.m[0][3] *= flScaleFactor;

        // Масштабируем строку 1 (Y-axis в Screen Space)
        matrix.m[1][0] *= flScaleFactor;
        matrix.m[1][1] *= flScaleFactor;
        matrix.m[1][2] *= flScaleFactor;
        matrix.m[1][3] *= flScaleFactor;

        // Строки 2 (Z/Depth) и 3 (W/Projection) оставляем без изменений.
    }

    bool WorldToScreen(const sdk::math::vector3& vecOrigin, sdk::math::vector3& vecScreen, const Matrix4x4& viewMatrix)
    {
        // 1. Считаем W-компонент (глубину для проекции)
        // Используем .m[3][x], так как напрямую [] у вашей структуры не работает
        const float flWidth = viewMatrix.m[3][0] * vecOrigin.x +
                              viewMatrix.m[3][1] * vecOrigin.y +
                              viewMatrix.m[3][2] * vecOrigin.z +
                              viewMatrix.m[3][3];

        // Проверка, что точка не за камерой (или слишком близко)
        if (flWidth < 0.001f)
            return false;

        // 2. Считаем инверсию для деления
        const float flInverse = 1.0f / flWidth;

        // 3. Проецируем координаты (умножаем X/Y вектора на матрицу и делим на W)
        vecScreen.x = (viewMatrix.m[0][0] * vecOrigin.x +
                       viewMatrix.m[0][1] * vecOrigin.y +
                       viewMatrix.m[0][2] * vecOrigin.z +
                       viewMatrix.m[0][3]) * flInverse;

        vecScreen.y = (viewMatrix.m[1][0] * vecOrigin.x +
                       viewMatrix.m[1][1] * vecOrigin.y +
                       viewMatrix.m[1][2] * vecOrigin.z +
                       viewMatrix.m[1][3]) * flInverse;

        // 4. Переводим из NDC (-1..1) в координаты экрана (пиксели)
        const ImVec2 vecDisplaySize = ImGui::GetIO().DisplaySize;

        vecScreen.x = (vecDisplaySize.x * 0.5f) + (vecScreen.x * vecDisplaySize.x) * 0.5f;
        vecScreen.y = (vecDisplaySize.y * 0.5f) - (vecScreen.y * vecDisplaySize.y) * 0.5f;

        return true;
    }

    // Расчет вертикального FOV из горизонтального (4:3 base)
    // Dota 2 использует 75 градусов по горизонтали для 4:3.
    float CalculateDotaFovY() {
        const float fovX_deg = 75.0f;
        const float aspect_4_3 = 4.0f / 3.0f;

        float halfFovX = XMConvertToRadians(fovX_deg / 2.0f);
        float tanHalfFovY = tanf(halfFovX) / aspect_4_3;
        return 2.0f * atanf(tanHalfFovY); // Результат ~1.043 rad (59.78 deg)
    }

    // Построение чистой матрицы вида и проекции для Dota 2
    XMMATRIX BuildPerfectDotaMatrix(const POINT& targetPos2D, float distance, ImVec2 screenSize) {
        // 1. Координаты цели (Virtual Camera Target)
        XMVECTOR target = XMVectorSet((float)targetPos2D.x, (float)targetPos2D.y, 0.0f, 0.0f); // Z=0 (земля)

        // 2. View Matrix
        // Углы: Pitch 60°, Yaw 90° (Camera looks North)
        const float pitchRad = XMConvertToRadians(60.0f);
        // Смещение камеры: назад по Y, вверх по Z
        // (При Yaw=90 камера стоит в -Y от цели)
        float yOffset = -distance * cosf(pitchRad);
        float zOffset = distance * sinf(pitchRad);

        XMVECTOR eyePos = XMVectorAdd(target, XMVectorSet(0.0f, yOffset, zOffset, 0.0f));
        XMVECTOR upVec = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

        XMMATRIX view = XMMatrixLookAtRH(eyePos, target, upVec);

        // 3. Projection Matrix
        static const float fovY = CalculateDotaFovY();
        float aspectRatio = screenSize.x / screenSize.y;

        XMMATRIX proj = XMMatrixPerspectiveFovRH(fovY, aspectRatio, 10.0f, 5000.0f);

        return XMMatrixMultiply(view, proj);
    }

    XMMATRIX CreateDotaViewMatrix(const XMVECTOR& target, float distance)
    {
        // Стандартные углы Dota 2: Pitch ~60 градусов, Yaw 90 (смотрит на Север)
        // Внимание: Углы могут немного отличаться, лучше взять из viewangles, если доступны.
        // Но для "стандартной" камеры фиксированный угол обычно работает отлично.
        const float pitch = XMConvertToRadians(60.0f);
        const float yaw   = XMConvertToRadians(90.0f);

        // Рассчитываем смещение камеры относительно цели (Target)
        // Камера сдвинута назад и вверх
        // В системе координат Source 2: Z - вверх.
        float offsetZ = distance * sinf(pitch);      // Высота
        float horizontalDist = distance * cosf(pitch); // Проекция на землю

        // Смещение по XY (зависит от Yaw, обычно камера просто сдвинута по Y вниз)
        // Если Yaw = 90 (смотрит по +Y), то камера находится в -Y
        float offsetX = 0.0f;
        float offsetY = -horizontalDist;

        XMVECTOR eyePos = XMVectorAdd(target, XMVectorSet(offsetX, offsetY, offsetZ, 0.0f));
        XMVECTOR upVec  = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f); // Z-up world

        // Создаем матрицу вида (Right-Handed для совместимости, проверьте LH если курсор инвертирован)
        return XMMatrixLookAtRH(eyePos, target, upVec);
    }

    XMMATRIX RebuildWorldProjection(XMMATRIX originalVP_DX, XMVECTOR currentCamPos, XMVECTOR virtualCamPos) {

    // 1. Генерируем View Matrix для ТЕКУЩЕЙ позиции (Reference)
    // Используем параметры Dota 2: LookAtRH
    // Внимание: Нужно точно знать точку, куда смотрит камера.
    // В Dota 2 камера смотрит под углом ~60 градусов вниз.
    // Проще всего восстановить View через Forward вектор, если он известен.
    // Но для зума мы можем сэмулировать LookAt.

    // Хак: Если мы не знаем точно Target, мы можем построить View от "Eye" (Pos)
    // в направлении "Forward" (которое фиксировано в Dota 2).
    // Forward vector in Dota: (0, 0.5, -0.866) примерно (60 deg pitch)

    // Альтернатива: Использовать стандартный "Dota View"
    static const XMVECTOR Up = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    static const XMVECTOR Forward = XMVectorSet(0.0f, 1.0f, -1.0f, 0.0f); // Примерно
    // Лучше взять углы из CUserCmd (viewangles) и сделать View матрицу через XMMatrixRotationRollPitchYaw

    // Допустим, мы используем упрощенный LookAt, считая что камера смотрит "перед собой" в землю
    // Чтобы код был надежным, давайте извлечем View из самой VP? Нет, это сложно.
    // Давайте построим View по известным данным:
    // Pos = currentCamPos.
    // Target = Pos + ForwardVector.

    // ВАЖНО: При патче зума (2000 -> 1200) меняется ТОЛЬКО позиция камеры вдоль вектора взгляда.
    // Углы (Rotation) остаются ТЕМИ ЖЕ.
    // Значит ViewMatrix отличается только позицией (Translation).

    // Вместо полной пересборки через Invert, можно сделать "Delta Transform":
    // 1. Узнаем вектор смещения: Delta = VirtualPos - RealPos
    // 2. Создаем матрицу трансляции на этот вектор.
    // 3. Применяем к VP.
    // В DX (Row-Major): NewVP = TranslationMatrix * OldVP ? Нет.
    // VP = View * Proj.
    // View = Translation * Rotation.
    // NewView = NewTranslation * Rotation.
    // Разница между ними — просто сдвиг.

    // Правильный способ модификации VP без инверсии (самый надежный):
    XMMATRIX translationDelta = XMMatrixTranslationFromVector(XMVectorSubtract(virtualCamPos, currentCamPos));

    // Поскольку VP = View * Proj, а View применяется первой (умножается слева на вектор),
    // то порядок: v * View * Proj.
    // Мы хотим v * (Delta * View) * Proj.
    // Значит нам нужно умножить Delta на View?
    // Нет. Translation применяется в World Space.
    // То есть: v_new = v * Delta.
    // Потом v_new * VP.
    // То есть NewVP = Delta * OldVP.

    return XMMatrixMultiply(translationDelta, originalVP_DX);
}

    bool WorldToScreen2(const XMFLOAT3& worldPos, const XMMATRIX& vpMatrix, const ImVec2& screenSize, ImVec2& outScreen)
    {
        XMVECTOR worldVec = XMLoadFloat3(&worldPos);
        XMVECTOR clip = XMVector3Transform(worldVec, vpMatrix);

        float w = XMVectorGetW(clip);
        if (w < 0.001f) return false;

        float x = XMVectorGetX(clip) / w;
        float y = XMVectorGetY(clip) / w;

        outScreen.x = (screenSize.x * 0.5f) * (x + 1.0f);
        outScreen.y = (screenSize.y * 0.5f) * (1.0f - y);
        return true;
    }

    XMMATRIX LoadSource2Matrix(float* ptr) {
        // 1. Загружаем как есть (Row-Major в памяти, но логически это Source формат)
        // Source: [ R R R Tx ]
        //         [ R R R Ty ]
        //         [ R R R Tz ]
        //         [ 0 0 0 1  ]
        XMMATRIX mat = XMLoadFloat4x4(reinterpret_cast<XMFLOAT4X4*>(ptr));

        // 2. Транспонируем, чтобы получить формат DirectX (Tx, Ty, Tz в нижней строке)
        // DX:     [ R R R 0 ]
        //         [ R R R 0 ]
        //         [ R R R 0 ]
        //         [ Tx Ty Tz 1 ]
        return XMMatrixTranspose(mat);
    }

    void UpdateWorldToProjectionMatrix(float* mtx, const sdk::math::vector3& oldCam, const sdk::math::vector3& newCam)
    {
        // 1. Extract the Camera "Back" vector from the matrix.
        // In a ViewProjection matrix, the 4th row (indices 12, 13, 14) typically corresponds
        // to the W-equation, which aligns with the negative forward vector (or positive back vector).
        // Based on the dump, Row 3 is [0, 0.5, -0.866], which matches -Forward.
        // We normalize it to be safe, though it should already be normalized.
        float backX = -mtx[12];
        float backY = -mtx[13];
        float backZ = -mtx[14];

        // Normalize vector (optional but recommended)
        float len = std::sqrt(backX * backX + backY * backY + backZ * backZ);
        if (len > 1e-6f) {
            backX /= len;
            backY /= len;
            backZ /= len;
        }

        // 2. Calculate the change in Camera Position (Eye) in World Space.
        // Camera Eye = LookAt + BackVector * Distance.
        // DeltaEye = DeltaLookAt + BackVector * DeltaDistance.

        float dDist = newCam.z - oldCam.z;
        float dX = newCam.x - oldCam.x;
        float dY = newCam.y - oldCam.y;

        // Movement due to X/Y (LookAt change) + Movement due to Distance (Zoom)
        // Assuming X/Y are ground coordinates (Z=0), the change is just (dX, dY, 0).
        float deltaEyeX = dX + backX * dDist;
        float deltaEyeY = dY + backY * dDist;
        float deltaEyeZ = 0.0f + backZ * dDist; // LookAt Z usually doesn't change

        // 3. Update the Matrix Translation Column (Indices 3, 7, 11, 15).
        // The translation part of the matrix (Col 3) is updated by subtracting the
        // projection of the movement vector onto the matrix's basis vectors.
        // Formula: Col3_new = Col3_old - M_3x3 * DeltaEye

        // Loop through 4 rows (0 to 3)
        for (int i = 0; i < 4; i++) {
            int rowStart = i * 4; // 0, 4, 8, 12

            // Dot product of the Row Vector (Rotation part) with the Delta Eye Vector
            float dot = mtx[rowStart + 0] * deltaEyeX +
                        mtx[rowStart + 1] * deltaEyeY +
                        mtx[rowStart + 2] * deltaEyeZ;

            // Subtract result from the Translation component (Column 3)
            mtx[rowStart + 3] -= dot;
        }
    }

    void StoreSource2Matrix(float* ptr, FXMMATRIX mat) {
        XMMATRIX out = XMMatrixTranspose(mat);
        XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(ptr), out);
    }

    void UpdateCameraMatrix(float* matrixPtr, const sdk::math::vector3& current, const sdk::math::vector3& target) {
        // 1. Загружаем матрицу и приводим к DX формату (Row-Major)
        XMMATRIX mat = LoadSource2Matrix(matrixPtr);

        // 2. Извлекаем вектор "Forward" (взгляд камеры).
        // В матрице ViewProjection (DX format) 4-й столбец (_14, _24, _34) отвечает за ось W в Clip Space.
        // Обычно W = -Z_view, поэтому этот вектор направлен ПРОТИВ взгляда (или по взгляду, зависит от знака).
        // Для Dota 2 вектор (_14, _24, _34) — это вектор, обратный взгляду (Back Vector) или Forward.
        // Исходя из дампа: _14=0, _24=0.5, _34=-0.866. Это вектор ВЗГЛЯДА (смотрит "в экран").
        // Нам нужен вектор "Back" (от земли к камере), чтобы двигать дистанцию.

        XMFLOAT4X4 fMat;
        XMStoreFloat4x4(&fMat, mat);

        // Вектор проекции (Forward)
        XMVECTOR forward = XMVectorSet(fMat._14, fMat._24, fMat._34, 0.0f);

        // Нормализуем на случай, если есть скейл
        forward = XMVector3Normalize(forward);

        // Вектор, вдоль которого меняется дистанция (назад от цели)
        XMVECTOR backDir = XMVectorNegate(forward);

        // 3. Считаем дельту движения камеры в мировых координатах.
        // Сдвиг по X/Y (LookAt)
        XMVECTOR deltaLookAt = XMVectorSet(target.x - current.x, target.y - current.y, 0.0f, 0.0f);

        // Сдвиг по дистанции (вдоль вектора Back)
        float deltaDist = target.z - current.z;
        XMVECTOR deltaZoom = XMVectorScale(backDir, deltaDist);

        // Итоговый сдвиг камеры (Delta Eye)
        XMVECTOR totalDelta = XMVectorAdd(deltaLookAt, deltaZoom);

        // 4. Корректируем матрицу.
        // Если мы сдвигаем камеру на Delta, это эквивалентно сдвигу мира на -Delta.
        // Новая проекция точки P: P' = VP * (P - Delta) = VP * P - (VP * Delta).
        // Значит, нам нужно вычесть (VP * Delta) из столбца трансляции.

        // В DX (Row-Major) трансляция находится в 4-й строке (mat.r[3]).
        // Нам нужно вычесть вектор, полученный умножением Delta на линейную часть матрицы.

        // Используем XMVector3TransformNormal, чтобы умножить вектор на 3x3 часть матрицы (без трансляции)
        XMVECTOR transformDelta = XMVector3TransformNormal(totalDelta, mat);

        // Вычитаем из строки трансляции
        mat.r[3] = XMVectorSubtract(mat.r[3], transformDelta);

        // 5. Сохраняем обратно в формат Source 2
        StoreSource2Matrix(matrixPtr, mat);
    }

    XMMATRIX BuildVirtualMatrix(const XMMATRIX& originalVP, float dx, float dy, float zoomDelta = 800.0f) {
        // 1. Применяем Pan (Сдвиг по X/Y)
        // Используем матрицу трансляции для смещения мира относительно камеры
        XMMATRIX panMatrix = XMMatrixTranslation(-dx, -dy, zoomDelta);
        XMMATRIX virtualVP = XMMatrixMultiply(panMatrix, originalVP);

        // 2. Применяем Zoom Hack (Сдвиг глубины)
        // Патчим элементы _43 (Z-Translation) и _44 (W-Translation) напрямую
        XMFLOAT4X4 vpData;
        XMStoreFloat4x4(&vpData, virtualVP);

        // Проверка безопасности: W не должен стать отрицательным или слишком малым
        // 50.0f - безопасный запас, чтобы избежать артефактов Near Plane
        //if (vpData._44 > zoomDelta + 50.0f) {
        ///    vpData._43 -= zoomDelta; // Смещаем Z (глубину)
        //    vpData._44 -= zoomDelta; // Смещаем W (перспективное деление)
        //}

        return XMLoadFloat4x4(&vpData);
    }

    struct Vector3 {
        float x, y, z;
    };

    // Функция трансформации матрицы камеры
    void TransformCameraMatrix(
        const Matrix4x4& sourceMatrix,
        const Vector3& sourcePos,
        float sourceDistance,
        const Vector3& targetPos,
        float targetDistance,
        Matrix4x4& outMatrix)
    {
        // Копируем исходную матрицу
        outMatrix = sourceMatrix;

        // Извлекаем rotation матрицу (первые 3x3 элемента)
        // Она постоянна для всех камер с одинаковыми углами

        // Вычисляем разницу позиций
        float deltaX = targetPos.x - sourcePos.x;
        float deltaY = targetPos.y - sourcePos.y;
        float deltaZ = targetPos.z - sourcePos.z;

        // Применяем rotation к дельте позиции (с инверсией знака)
        // Translation = R * (-deltaPos)
        float rotatedDeltaX = sourceMatrix.m[0][0] * (-deltaX) +
                              sourceMatrix.m[0][1] * (-deltaY) +
                              sourceMatrix.m[0][2] * (-deltaZ);

        float rotatedDeltaY = sourceMatrix.m[1][0] * (-deltaX) +
                              sourceMatrix.m[1][1] * (-deltaY) +
                              sourceMatrix.m[1][2] * (-deltaZ);

        float rotatedDeltaZ = sourceMatrix.m[2][0] * (-deltaX) +
                              sourceMatrix.m[2][1] * (-deltaY) +
                              sourceMatrix.m[2][2] * (-deltaZ);

        // Обновляем translation (последний столбец, первые 3 строки)
        outMatrix.m[0][3] = sourceMatrix.m[0][3] + rotatedDeltaX;
        outMatrix.m[1][3] = sourceMatrix.m[1][3] + rotatedDeltaY;
        outMatrix.m[2][3] = sourceMatrix.m[2][3] + rotatedDeltaZ;

        // Корректируем Z компонент на изменение distance
        float deltaDistance = targetDistance - sourceDistance;
        outMatrix.m[2][3] += deltaDistance;

        // Обновляем W компонент (4-я строка, последний столбец)
        // Используется для перспективной проекции в WorldToScreen
        float rotatedDeltaW = sourceMatrix.m[3][0] * (-deltaX) +
                              sourceMatrix.m[3][1] * (-deltaY) +
                              sourceMatrix.m[3][2] * (-deltaZ);

        outMatrix.m[3][3] = sourceMatrix.m[3][3] + rotatedDeltaW + deltaDistance;
    }

    void C_FeatureCameraEmulator::onCreateMove(sdk::datatype::user_cmd_s* userCmd)
    {
        return;

        if (!this->dummyCamera_.has_value()) {
            assert("You must load the module before `level init`!");
            return;
        }

        auto& cmd = userCmd->cmd;

        auto currentCam = C_ServiceLocator::getInstance<sdk::singleton::C_DotaCamaraManager>()->getPrimaryCamera();
        auto currentCamPos = currentCam->getCameraPosition();

        this->dummyCamera_->setLookAtPos({static_cast<float>(currentCamPos.x), static_cast<float>(currentCamPos.y)});

        auto pos = this->dummyCamera_->getCameraPosition();
        dbg("%f %f %f", pos.x, pos.y, pos.z);

        ImVec2 screenSize = ImGui::GetIO().DisplaySize;
        auto pRenderSystem = C_ServiceLocator::getInstance<sdk::singleton::C_RenderGameSystem>();
        auto originalMtx = reinterpret_cast<float*>(pRenderSystem->getWorldProjectionMatrix());
        Matrix4x4 vp; memcpy(&vp, originalMtx, sizeof(Matrix4x4));

        Matrix4x4 vp2;
        TransformCameraMatrix(vp, {static_cast<float>(currentCamPos.x), static_cast<float>(currentCamPos.y), currentCamPos.z}, 2000, {pos.x, pos.y, pos.z}, 1200, vp2);

        sdk::math::vector3 newCam{};
        WorldToScreen({cmd.crosshairtrace().x(), cmd.crosshairtrace().y(), cmd.crosshairtrace().z()}, newCam, vp2);
        //WorldToScreen2(XMFLOAT3{cmd.crosshairtrace().x(), cmd.crosshairtrace().y(), cmd.crosshairtrace().z()}, tmp, screenSize, newCamPos);

        auto quantizedPosition = sdk::math::QuantizePoint(POINT(screenSize.x, screenSize.y), POINT(newCam.x, newCam.y));

        cmd.set_cameraposition_x(pos.x);
        cmd.set_cameraposition_y(pos.y);
        cmd.mutable_base()->set_mousedx(quantizedPosition.x);
        cmd.mutable_base()->set_mousedy(quantizedPosition.y);

        return;

        /*ImVec2 screenSize = ImGui::GetIO().DisplaySize;

        sdk::math::vector3 originalCamera = { static_cast<float>(cmd.cameraposition_x()), static_cast<float>(cmd.cameraposition_y()), 2000.0f };
        this->virtualCamera_ = { static_cast<float>(cmd.cameraposition_x()), static_cast<float>(cmd.cameraposition_y()), 1200.0f };

        // 2. Load Original Matrix
        auto pRenderSystem = C_ServiceLocator::getInstance<sdk::singleton::C_RenderGameSystem>();
        auto originalMtx = reinterpret_cast<float*>(pRenderSystem->getWorldProjectionMatrix());
        auto mtx = reinterpret_cast<float*>(pRenderSystem->getWorldProjectionMatrix());
        if (!originalMtx) return;

        dbg("%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f", mtx[0], mtx[1], mtx[2], mtx[3], mtx[4], mtx[5], mtx[6], mtx[7], mtx[8], mtx[9], mtx[10], mtx[11], mtx[12], mtx[13], mtx[14], mtx[15]);

        cmd.set_cameraposition_x(this->virtualCamera_.x);
        cmd.set_cameraposition_y(this->virtualCamera_.y);

        auto tmp = BuildVirtualMatrix(LoadSource2Matrix(originalMtx), 0, 0, 800.f);

        sdk::math::vector3 newCam{}; ImVec2 newCamPos{};
        //WorldToScreen({cmd.crosshairtrace().x(), cmd.crosshairtrace().y(), cmd.crosshairtrace().z()}, newCam, tmp2);
        WorldToScreen2(XMFLOAT3{cmd.crosshairtrace().x(), cmd.crosshairtrace().y(), cmd.crosshairtrace().z()}, tmp, screenSize, newCamPos);

        auto quantizedPosition = sdk::math::QuantizePoint(POINT(screenSize.x, screenSize.y), POINT(newCamPos.x, newCamPos.y));
        cmd.mutable_base()->set_mousedx(quantizedPosition.x);
        cmd.mutable_base()->set_mousedy(quantizedPosition.y);*/
    }

    bool C_FeatureCameraEmulator::initialize()
    {
        const auto hookDispatcher = C_ServiceLocator::getInstance<hook::C_HookDispatcher>();

        hookDispatcher->subscribe<sdk::datatype::user_cmd_s*>(
            static_cast<hook::hook_id_t>(hook::impl::hook_impl_type_e::CREATE_MOVE),
            [this](sdk::datatype::user_cmd_s* cmd){
                onCreateMove(cmd);
            }
        );

        hookDispatcher->subscribe(
            static_cast<hook::hook_id_t>(hook::impl::hook_impl_type_e::ON_LEVEL_INIT),
            [this]{ onLevelInit(); }
        );

        return true;
    }
} // feature