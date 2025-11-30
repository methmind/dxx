//
// Created by sexey on 25.11.2025.
//

#include "renderer_frame.h"

namespace render
{
    void C_RendererFrame::render(ImDrawList* drawList)
    {
        this->renderFrame_ = this->intermediateFrame_.exchange(this->renderFrame_,
            std::memory_order_acq_rel
        );

        for (const auto& cmd : *this->renderFrame_) {
            if (cmd) {
                cmd(drawList);
            }
        }
    }

    void C_RendererFrame::enqueue(const on_draw_callback& cmd) const
    {
        this->writeFrame_->emplace_back(cmd);
    }

    void C_RendererFrame::bake()
    {
        this->writeFrame_ = this->intermediateFrame_.exchange(this->writeFrame_,
            std::memory_order_acq_rel
        );

        // Capacity still with us
        this->writeFrame_->clear();
    }
} // renderer