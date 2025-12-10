//
// Created by sexey on 11.11.2025.
//

#ifndef DXX_DLC_SDK_SIGNATURE_H
#define DXX_DLC_SDK_SIGNATURE_H

namespace sdk::signature
{
    // sub_181B8E900()
    constexpr auto GET_VIEW_RENDER_INSTANCE_FUNC = "48 8D 05 ? ? ? ? C3 CC CC CC CC CC CC CC CC 85 D2 78";

    // Offset is (instance + 0x20F0)
    constexpr auto EGS_NUMBER_OF_ENTITIES = "80 FA ? 75 ? 8B 81";

    // sub_18137FA10(__int64 a1, int a2)
    constexpr auto EGS_GET_BASE_ENTITY = "4C 8D 49 ? 81 FA ? ? ? ? 77";

    // "game_newmap" -> "mapname"
    constexpr auto ON_LEVEL_INIT_CALLBACK = "40 55 56 41 56 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 0D";

    // "map_shutdown"
    constexpr auto ON_LEVEL_SHUTDOWN_CALLBACK = "48 83 EC ? 48 8B 0D ? ? ? ? 48 8D 15 ? ? ? ? 45 33 C9 45 33 C0 48 8B 01 FF 50 ? 48 85 C0 74 ? 48 8B 0D ? ? ? ? 48 8B D0 4C 8B 01 41 FF 50 ? 48 83 C4";

    // "CDOTA_MinimapRenderer::RenderCreepCamps"
    constexpr auto GET_HUD_ICONS = "48 8D 05 ? ? ? ? C3 CC CC CC CC CC CC CC CC 48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 41 56 48 83 EC ? 48 8B D9";

    /*
     * __int64 __fastcall sub_18155D470(float *a1, __int64 a2)
     * Can be found via xref on g_pRenderGameSystem
    * __int64 __fastcall ScreenTransform(float *a1, __int64 a2)
      {
        unsigned int v3; // r14d
        float *v4; // rax
        float v5; // xmm0_4
        float v6; // xmm1_4
        float v7; // xmm2_4
        float v8; // xmm1_4

        v3 = 0;
        v4 = (float *)(*(__int64 (__fastcall **)(__int64, _QWORD))(*(_QWORD *)g_pRenderGameSystem + 320LL))(
                        g_pRenderGameSystem,
                        0);
        v5 = (float)((float)(v4[2] * a1[2]) + (float)((float)(v4[1] * a1[1]) + (float)(*v4 * *a1))) + v4[3];
        *(float *)a2 = v5;
        v6 = (float)((float)(v4[6] * a1[2]) + (float)((float)(v4[5] * a1[1]) + (float)(v4[4] * *a1))) + v4[7];
        *(float *)(a2 + 4) = v6;
        v7 = (float)((float)(v4[14] * a1[2]) + (float)((float)(v4[13] * a1[1]) + (float)(v4[12] * *a1))) + v4[15];
        *(_DWORD *)(a2 + 8) = 0;
        if ( v7 >= 0.001 )
        {
          *(float *)a2 = v5 * (float)(1.0 / v7);
          v8 = v6 * (float)(1.0 / v7);
        }
        else
        {
          *(float *)a2 = v5 * 100000.0;
          v8 = v6 * 100000.0;
          v3 = 1;
        }
        *(float *)(a2 + 4) = v8;
        return v3;
      }
     */
    constexpr auto SCREEN_TRANSFORM_FUNC = "48 89 5C 24 ? 57 48 83 EC ? 48 8B F9 48 8B DA 48 8B 0D ? ? ? ? 48 85 C9 0F 84 ? ? ? ? 48 8B 01";

    // Can be found via "IGameSystem::InitAllSystems" and "Game System %s is defined twice!\n"
    constexpr auto GAME_SYSTEM_INIT_ALL_FUNC = "48 89 5C 24 ? 55 56 57 48 83 EC ? 48 8D 05 ? ? ? ? 48 C7 44 24 ? ? ? ? ? 33 F6";

    constexpr auto GAME_SYSTEM_FACTORY_MOV_OPCODE = "48 8B 1D ? ? ? ? 48 85 DB 0F 84 ? ? ? ? BD";

    // Can be found via xref on "OnColorChanged"
    constexpr auto BASE_MODEL_ENTITY_CHANGE_COLOR_FUNC = "40 53 48 83 EC ? 48 8B D9 48 8B 89 ? ? ? ? 48 8B 01 0F B6 93";

    /*
     * CInput::CreateMove == 5 VMT index
     * void(__fastcall*)(void* cinput, int32_t slot, bool isActive);
     */
    constexpr auto CINPUT_CREATE_MOVE_FUNC = "85 D2 0F 85 ? ? ? ? 48 8B C4 44 88 40";

    /*
     * Can be found in CInput::CreateMove
     * void*(__fastcall*)(void* playerController, int32_t sequenceNumber);
     */
    constexpr auto CINPUT_GET_USER_CMD_FUNC = "40 53 48 83 EC ? 8B DA E8 ? ? ? ? 4C 8B C0";

    /*
     * Can be found in CInput::CreateMove
     * void*(__fastcall*)(int32_t playerID);
     */
    constexpr auto GET_PLAYER_CONTROLLER_FUNC = "33 C0 83 F9 ? 0F 44 C8";

    /*
     * Can be found in CInput::CreateMove
     * void(__fastcall*)(void* playerController, int32_t& slot);
     */
    constexpr auto GET_PLAYER_SCREEN_ID_FUNC = "48 83 EC ? 4C 8B 0D ? ? ? ? 4C 8B DA";

    /*
     * Can be found in CInput::CreateMove
     * void*(__fastcall*)(void* cmdCircularBuffer, int32_t screenID);
     */
    constexpr auto GET_PLAYER_CMD_BUFFER_FUNC = "48 89 4C 24 ? 41 56 41 57";

    /*
     * Can be found in CInput::CreateMove
     * mov rcx, cs:off_18XXXXXXX
     */
    constexpr auto MOV_RCX_GLOBAL_CMD_CIRCULAR_BUFFER = "48 8B 0D ? ? ? ? E8 ? ? ? ? 49 8B CF";

    /*
     * Can be found in CInput::CreateMove
     * mov r14d, [rax+5460h]
     */
    constexpr auto ADD_RAX_SEQUENCE_NUMBER_OFFSET = "44 8B B0 ? ? ? ? 41 8B D6";
}

#endif //DXX_DLC_SDK_SIGNATURE_H