//
// Created by sexey on 13.02.2026.
//
module;
#include <windows.h>
#include <memory>

#include "debug/debug_output.h"

export module dxx;

import bootstrap;

export namespace dxx
{
    class C_Application
    {
    public:
        ~C_Application() = default;

        C_Application() = default;

        void entry()
        {
            const auto services = bootstrap::Build();
            if (!services) {
                dbg("bootstrap::Build got:err = Unable to build application!");
                return;
            }

            Sleep(999999);
        }

    private:
        //todo
    };
}
