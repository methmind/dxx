//
// Created by sexey on 28.02.2026.
//
module;
#include <memory>

export module bootstrap.gui;

import service.container;
import gui.regedit;

import worker_queue;

export namespace bootstrap
{
    using gui_renderer_queue_t = C_WorkerQueue;

    bool InitializeGUI(const std::unique_ptr<C_ServiceContainer>& services)
    {
        services->add<gui::C_WidgetRegedit>();
        services->add<gui_renderer_queue_t>();
        return true;
    }
}
