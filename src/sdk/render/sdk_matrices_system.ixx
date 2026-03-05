//
// Created by sexey on 28.02.2026.
//
module;
#include "SimpleMath.h"

export module sdk.matrices_system;

import service.locator;
import hook.dispatcher;
import hook.type;

import sdk.math.matrix;

namespace sdk
{
    export class C_MatricesSystem
    {
    public:
        ~C_MatricesSystem() = default;

        C_MatricesSystem()
        {
            this->onMatricesUpdateSubscription_ = C_ServiceLocator::Get<hook::C_HookDispatcher>()->subscribe<void*, void*, void*, void*, void*, void*>(
              static_cast<hook::hook_id_t>(hook::hook_type_e::ON_MATRICES_UPDATE),
              [this](void* renderSystem, void* viewRender, void* worldToView, void* viewToProjection, void* worldToProjection, void* worldToScreen) {
                  onMatricesUpdate(static_cast<matrix_t*>(worldToView), static_cast<matrix_t*>(worldToProjection));
              }
            );
        }

        [[nodiscard]] const matrix_t& getWorldToView() const { return this->worldToView_; }

        [[nodiscard]] const matrix_t& getWorldToProjection() const { return this->worldToProjection_; }

    private:
        void onMatricesUpdate(const matrix_t* worldToView, const matrix_t* worldToProjection)
        {
            worldToView->Transpose(this->worldToView_);
            worldToProjection->Transpose(this->worldToProjection_);
        }

        matrix_t worldToView_;
        matrix_t worldToProjection_;

        hook::hook_subscription_t onMatricesUpdateSubscription_;
    };
}