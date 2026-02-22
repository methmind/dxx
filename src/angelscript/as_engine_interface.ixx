//
// Created by sexey on 22.02.2026.
//
module;
#include "angelscript.h"

export module as.engine_interface;

import as.context_pool;

export namespace as
{
    constexpr auto ENGINE_USERDATA_ID = 0xBADC0DE;

    class C_IASEngine
    {
    public:
        virtual ~C_IASEngine() = default;

        [[nodiscard]] virtual asIScriptEngine* getEngine() = 0;

        [[nodiscard]] virtual as_context_t getContext() const = 0;
    };
}
