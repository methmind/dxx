//
// Created by sexey on 25.11.2025.
//

#ifndef DXX_DLC_RENDERER_FRAME_H
#define DXX_DLC_RENDERER_FRAME_H

#include <atomic>
#include <functional>

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

        using draw_queue = std::vector<on_draw_callback>;

    private:
        draw_queue* renderFrame_;
        std::atomic<draw_queue*> intermediateFrame_;
        draw_queue* writeFrame_;

    public:

        void render(ImDrawList* drawList);

        void enqueue(const on_draw_callback& cmd) const;

        void bake();

        C_RendererFrame() :
            renderFrame_(new draw_queue()),
            intermediateFrame_(new draw_queue()),
            writeFrame_(new draw_queue()) {
            this->renderFrame_->reserve(DEFAULT_QUEUE_SIZE);
            this->intermediateFrame_.load()->reserve(DEFAULT_QUEUE_SIZE);
            this->writeFrame_->reserve(DEFAULT_QUEUE_SIZE);
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