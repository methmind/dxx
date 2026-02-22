//
// Created by sexey on 22.02.2026.
//
export module sdk.datatype.client_frame_stage_t;

namespace sdk
{
    export enum ClientFrameStage_t
    {
        FRAME_START = -1,
        FRAME_NET_UPDATE_START,
        FRAME_NET_UPDATE_POSTDATAUPDATE_START,
        FRAME_NET_UPDATE_POSTDATAUPDATE_END,
        FRAME_NET_FULL_FRAME_UPDATE_ON_REMOVE,
        FRAME_RENDER_START,
        FRAME_RENDER_END,
        FRAME_NET_UPDATE_END,
        FRAME_NET_CREATION,
        FRAME_SIMULATE_END
    };
}