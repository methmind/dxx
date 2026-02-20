//
// Created by sexey on 16.02.2026.
//
module;
#include <atomic>
#include <functional>
#include <vector>

#include "imgui.h"

export module renderer.frame;

namespace render
{
    export using on_draw_callback_t = std::function<void(ImDrawList*)>;

    export using draw_queue_t = std::vector<on_draw_callback_t>;

    constexpr size_t DEFAULT_QUEUE_SIZE = 128;

    struct tagged_pointer_s
    {
        static constexpr uintptr_t TAG_BITS = 3;
        // 8 возможных значений тега
        static constexpr uintptr_t TAG_MASK = (1ULL << TAG_BITS) - 1;
        static constexpr uintptr_t PTR_MASK = ~TAG_MASK;

        uintptr_t value;

        tagged_pointer_s() : value(0) {}

        explicit tagged_pointer_s(draw_queue_t* ptr, uintptr_t tag = 0)
            : value(reinterpret_cast<uintptr_t>(ptr) | (tag & TAG_MASK)) {}

        [[nodiscard]] draw_queue_t* ptr() const
        {
            return reinterpret_cast<draw_queue_t*>(value & PTR_MASK);
        }

        [[nodiscard]] uintptr_t tag() const
        {
            return value & TAG_MASK;
        }
    };

    export class C_RendererFrame
    {
    public:
        C_RendererFrame() :
            renderQueue_(new draw_queue_t()),
            writeQueue_(new draw_queue_t()),
            intermediateQueue_(tagged_pointer_s(new draw_queue_t(), 0)),
            lastRenderedEpoch_(tagged_pointer_s::TAG_MASK)
        {
            this->renderQueue_->reserve(DEFAULT_QUEUE_SIZE);
            this->writeQueue_->reserve(DEFAULT_QUEUE_SIZE);
            this->intermediateQueue_.load().ptr()->reserve(DEFAULT_QUEUE_SIZE);
        }

        ~C_RendererFrame()
        {
            delete this->renderQueue_;
            delete this->writeQueue_;
            delete this->intermediateQueue_.load().ptr();
        }

        void render(ImDrawList* drawList)
        {
            if (const auto state = this->intermediateQueue_.load(std::memory_order_acquire); state.tag() != this->lastRenderedEpoch_)
            {
                // Отдаем свой отработанный буфер в обмен на свежий.
                const tagged_pointer_s emptyState(this->renderQueue_, state.tag());
                const auto oldState = this->intermediateQueue_.exchange(emptyState, std::memory_order_acq_rel);

                // Забираем себе свежую очередь и запоминаем её эпоху
                this->renderQueue_ = oldState.ptr();
                this->lastRenderedEpoch_ = oldState.tag();
            }
            // ИНАЧЕ: Оставляем старый renderQueue_ нетронутым (спасает от мерцания)

            for (const auto& cmd : *this->renderQueue_) {
                if (cmd) {
                    cmd(drawList);
                }
            }
        }

        void enqueue(on_draw_callback_t cmd) const
        {
            this->writeQueue_->emplace_back(std::move(cmd));
        }

        void bake()
        {
            this->currentBakeEpoch_ = (this->currentBakeEpoch_ + 1) & tagged_pointer_s::TAG_MASK;

            // Готовим новый указатель со свежими данными и новой эпохой
            const tagged_pointer_s newState(this->writeQueue_, this->currentBakeEpoch_);
            const auto oldState = this->intermediateQueue_.exchange(newState, std::memory_order_acq_rel);

            // Забранный буфер становится нашим новым writeQueue_ для следующего кадра
            this->writeQueue_ = oldState.ptr();
            this->writeQueue_->clear();
        }

    private:
        draw_queue_t* renderQueue_;
        draw_queue_t* writeQueue_;
        std::atomic<tagged_pointer_s> intermediateQueue_;

        uintptr_t currentBakeEpoch_ = 0;
        uintptr_t lastRenderedEpoch_;
    };
}
