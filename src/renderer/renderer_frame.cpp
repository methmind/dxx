//
// Created by sexey on 25.11.2025.
//

#include "renderer_frame.h"

namespace render
{
    void C_RendererFrame::render(ImDrawList* drawList)
    {
        // Fix for ABA/Jitter: Only exchange if intermediate frame is newer
        Frame* intermediate = this->intermediateFrame_.load(std::memory_order_acquire);
        
        if (intermediate->sequence > this->renderFrame_->sequence) {
             this->renderFrame_ = this->intermediateFrame_.exchange(this->renderFrame_,
                std::memory_order_acq_rel
             );
        }

        for (const auto& cmd : this->renderFrame_->commands) {
            if (cmd) {
                cmd(drawList);
            }
        }
    }

    void C_RendererFrame::enqueue(const on_draw_callback& cmd) const
    {
        this->writeFrame_->commands.emplace_back(cmd);
    }

    void C_RendererFrame::bake()
    {
        // Increment sequence to mark this frame as newer
        this->writeFrame_->sequence = ++this->frameSequenceCounter_;

        this->writeFrame_ = this->intermediateFrame_.exchange(this->writeFrame_,
            std::memory_order_acq_rel
        );

        // Capacity still with us
        this->writeFrame_->commands.clear();
    }
} // renderer