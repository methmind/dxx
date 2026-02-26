//
// Created by sexey on 19.02.2026.
//
module;
#include <functional>
#include <vector>

#include "cs_plain_guarded.h"

export module renderer.queue;

namespace render
{
    using command_list_t = std::vector<std::function<void()>>;

    export class C_RendererQueue
    {
    public:
        C_RendererQueue() = default;

        template<typename func_t>
        void enqueue(func_t&& cmd)
        {
            this->commands_.lock()->emplace_back(std::forward<func_t>(cmd));
        }

        void processCommands()
        {
            command_list_t pendingCommands;
            {
                const auto guarded = this->commands_.lock();
                if (guarded->empty()) {
                    return;
                }

                pendingCommands = std::move(*guarded);
            }

            for (const auto& cmd : pendingCommands) {
                cmd();
            }
        }

    private:
        libguarded::plain_guarded<command_list_t> commands_;
    };
}
