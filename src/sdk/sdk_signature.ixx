//
// Created by sexey on 28.02.2026.
//
export module sdk.signature;

export namespace sdk
{
    /*
     * Can be found in CDOTAViewRender::OnRenderStart
     * void*(__fastcall*)(void* renderGameSystem, void* viewRender, void* worldToView, void* viewToProjection, void* worldToProjection, void* worldToPixels);
    */
    constexpr auto GET_MATRICES_FOR_VIEW = "48 8B C4 48 89 68 ? 48 89 70 ? 57 48 81 EC ? ? ? ? 0F 29 70";
}