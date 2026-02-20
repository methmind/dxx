//
// Created by sexey on 13.02.2026.
//
module;
#include <windows.h>
#include <memory>

#include "debug/debug_output.h"

export module dxx;

import bootstrap;
import bind_system;

export namespace dxx
{
    class C_Application
    {
    public:
        ~C_Application() = default;

        C_Application() = default;

        void entry()
        {
            if (this->closeEvent_ = CreateEventW(nullptr, false, false, nullptr); !this->closeEvent_) {
                dbg("CreateEvent got:err = {}", GetLastError());
                return;
            }

            const auto services = bootstrap::Build();
            if (!services) {
                dbg("bootstrap::Build got:err = Unable to build application!");
                return;
            }

            auto eventSubscription = services->get<input::C_BindSystem>()->bind(VK_END,
                [this] {
                    SetEvent(this->closeEvent_);
                }
            );

            if (const auto err = WaitForSingleObject(this->closeEvent_, INFINITE); err != WAIT_OBJECT_0) {
                dbg("WaitForSingleObject got:err = {}:{}", err, GetLastError());
            }
        }

    private:
        HANDLE closeEvent_;
    };
}
