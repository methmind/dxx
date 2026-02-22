//
// Created by sexey on 22.02.2026.
//
module;

export module sdk.engine.source2_client;

namespace sdk
{
    /*
     * 48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B F1 33 FF 48 8B 89
     * void(__fastcall*)(void* instance, ClientFrameStage_t);
     * "C:\\buildworker\\source2_dota_rel_2019_win64\\build\\src\\game\\client\\cdll_client_int.cpp" - xref to this
     */
    constexpr auto FRAME_STAGE_NOTIFY_VMT_INDEX = 36;

    export class C_Source2Client
    {
    public:
        ~C_Source2Client() = default;

        C_Source2Client() = default;

        [[nodiscard]] void* getFrameStageNotify()
        {
            const auto vtable = *reinterpret_cast<void***>(this);
            return vtable[FRAME_STAGE_NOTIFY_VMT_INDEX];
        }
    };
}