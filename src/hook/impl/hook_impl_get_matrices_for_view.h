//
// Created by sexey on 18.12.2025.
//

#ifndef DXX_DLC_HOOK_IMPL_GET_MATRICES_FOR_VIEW_H
#define DXX_DLC_HOOK_IMPL_GET_MATRICES_FOR_VIEW_H

namespace hook::impl
{
    void* __fastcall hkGetMatricesForView(void* renderGameSystem, void* viewRender, void* worldToView, void* viewToProjection, void* worldToProjection, void* worldToPixels);
} // hook

#endif //DXX_DLC_HOOK_IMPL_GET_MATRICES_FOR_VIEW_H