//
// Created by sexey on 18.12.2025.
//

#include "sdk_matrices_system.h"

#include "hook/hook_dispatcher.h"
#include "hook/impl/hook_impl_type.h"
#include "service_locator/service_container.h"
#include "service_locator/service_locator.h"

namespace sdk::custom
{
    void C_MatricesSystem::onGetMatricesFunc(math::matrix4x4_s* viewWorld, math::matrix4x4_s* worldProjection, math::matrix4x4_s* worldPixel)
    {
        this->viewMatrix_ = *reinterpret_cast<DirectX::SimpleMath::Matrix*>(viewWorld);
        this->worldPixelMatrix_ = *reinterpret_cast<DirectX::SimpleMath::Matrix*>(worldPixel);
        this->worldProjectionMatrix_ = *reinterpret_cast<DirectX::SimpleMath::Matrix*>(worldProjection);
        //reinterpret_cast<DirectX::SimpleMath::Matrix*>(viewWorld)->Transpose(this->viewMatrix_);
        //reinterpret_cast<DirectX::SimpleMath::Matrix*>(worldProjection)->Transpose(this->worldProjectionMatrix_);
        //reinterpret_cast<DirectX::SimpleMath::Matrix*>(worldPixel)->Transpose(this->worldPixelMatrix_);
    }

    bool C_MatricesSystem::initialize()
    {
        C_ServiceLocator::getInstance<hook::C_HookDispatcher>()->
        subscribe<void*, void*, math::matrix4x4_s*, math::matrix4x4_s*, math::matrix4x4_s*, math::matrix4x4_s*>(
            static_cast<hook::hook_id_t>(hook::impl::hook_impl_type_e::GET_MATRICES_FOR_VIEW),
            [this](void* renderSystem, void* viewRender, math::matrix4x4_s* worldView,
                math::matrix4x4_s* viewProjection, math::matrix4x4_s* worldProjection, math::matrix4x4_s* worldPixel) {
                onGetMatricesFunc(worldView, worldProjection, worldPixel);
            }
        );

        return true;
    }
} // sdk