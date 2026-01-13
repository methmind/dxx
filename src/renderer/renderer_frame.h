//
// Created by sexey on 25.11.2025.
//

#ifndef DXX_DLC_RENDERER_FRAME_H
#define DXX_DLC_RENDERER_FRAME_H

#include <atomic>
#include <functional>
#include <vector>
#include <cstdint>

#include "imgui.h"

namespace render
{
    constexpr auto DEFAULT_QUEUE_SIZE = 128;

    /*
     * Only for SPSC (single-producer, single-consumer)
     */
    class C_RendererFrame
    {
    public:
        using on_draw_callback = std::function<void(ImDrawList*)>;

        struct Frame {
            std::vector<on_draw_callback> commands;
            uint64_t sequence = 0;

            Frame() {
                commands.reserve(DEFAULT_QUEUE_SIZE);
            }
        };

    private:
        Frame* renderFrame_;
        std::atomic<Frame*> intermediateFrame_;
        Frame* writeFrame_;
        
        uint64_t frameSequenceCounter_ = 0;

    public:

        void render(ImDrawList* drawList);

        void enqueue(const on_draw_callback& cmd) const;

        void bake();

        C_RendererFrame() :
            renderFrame_(new Frame()),
            intermediateFrame_(new Frame()),
            writeFrame_(new Frame()) {
        }

        ~C_RendererFrame()
        {
            delete this->renderFrame_;
            delete this->intermediateFrame_.load();
            delete this->writeFrame_;
        }
    };
} // renderer

#endif //DXX_DLC_RENDERER_FRAME_H