//
// Created by sexey on 29.11.2025.
//

#ifndef DXX_DLC_LUA_BINDING_RENDERER_H
#define DXX_DLC_LUA_BINDING_RENDERER_H

#include "lua_binding_interface.h"
#include "renderer/renderer.h"

namespace lua::binding
{
    constexpr auto RENDERER_NAMESPACE_NAME = "renderer";

    class C_LuaBindingRenderer final : public C_ILuaBinding
    {
    private:
        std::shared_ptr<render::C_Renderer> renderer_;

        static void RegisterRendererPrimitivesFrame(sol::state& state);

    public:

        bool apply(const std::weak_ptr<C_ILuaGuardedState>& guardedState) override;

        explicit C_LuaBindingRenderer(const std::shared_ptr<render::C_Renderer>& renderer) : renderer_(renderer) {}

        ~C_LuaBindingRenderer() override = default;
    };
} // lua

#endif //DXX_DLC_LUA_BINDING_RENDERER_H
