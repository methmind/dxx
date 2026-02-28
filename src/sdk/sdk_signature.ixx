//
// Created by sexey on 28.02.2026.
//
export module sdk.signature;

export namespace sdk
{
    // "game_newmap" -> "mapname"
    constexpr auto ON_LEVEL_INIT_CALLBACK = "40 55 56 41 56 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 0D";

    // "map_shutdown"
    constexpr auto ON_LEVEL_SHUTDOWN_CALLBACK = "48 83 EC ? 48 8B 0D ? ? ? ? 48 8D 15 ? ? ? ? 45 33 C9 45 33 C0 48 8B 01 FF 50 ? 48 85 C0 74 ? 48 8B 0D ? ? ? ? 48 8B D0 4C 8B 01 41 FF 50 ? 48 83 C4";

    /*
     * Can be found in CDOTAViewRender::OnRenderStart
     * void*(__fastcall*)(void* renderGameSystem, void* viewRender, void* worldToView, void* viewToProjection, void* worldToProjection, void* worldToPixels);
    */
    constexpr auto GET_MATRICES_FOR_VIEW = "48 8B C4 48 89 68 ? 48 89 70 ? 57 48 81 EC ? ? ? ? 0F 29 70";
}