//
// Created by sexey on 17.02.2026.
//
module;
#include "angelscript.h"

export module as.binding;

namespace as
{
    export class C_IASBinding
    {
    public:
        virtual ~C_IASBinding() = default;

        virtual void apply(asIScriptEngine* engine) = 0;
    };
}