//
// Created by sexey on 25.12.2025.
//

#ifndef DXX_DLC_LUA_BINDING_DYNAMIC_CAST_H
#define DXX_DLC_LUA_BINDING_DYNAMIC_CAST_H

#include <functional>

#include "hash/xxhash_wrapper.h"
#include "sol/sol.hpp"

namespace lua::binding::util
{
    template <typename base_t>
    class C_LuaBindingDynamicCast
    {
    public:
        using caster_func_t = std::function<sol::object(sol::state_view&, base_t*)>;

    private:
        std::unordered_map<std::string, caster_func_t, xx_hashier_s, std::equal_to<>> casterList_;

    public:

        template<typename T>
        void add(const std::string_view& name)
        {
            this->casterList_.emplace(
                std::make_pair(name, [](sol::state_view lua, base_t* ptr) -> sol::object {
                    return sol::make_object(lua, static_cast<T*>(ptr));
                })
            );
        }

        auto cast(sol::state_view state, base_t* instance, const std::string_view& name)
        {
            if (!instance) {
                luaL_error(state.lua_state(), "Attempting to cast a null pointer!");
                __builtin_unreachable();
            }

            const auto it = this->casterList_.find(name);
            if (it == this->casterList_.end()) {
                luaL_error(state.lua_state(), "Unable to cast entity!");
                __builtin_unreachable();
            }

            auto casted = it->second(state, instance);
            if (!casted.valid()) {
                luaL_error(state.lua_state(), "Unable to cast entity!");
                __builtin_unreachable();
            }

            return casted;
        }

        caster_func_t find(const std::string_view& name)
        {
            auto it = this->casterList_.find(name);
            if (it == this->casterList_.end()) {
                return nullptr;
            }

            return it->second;
        }

        C_LuaBindingDynamicCast() = default;

        ~C_LuaBindingDynamicCast() = default;
    };
}

#endif //DXX_DLC_LUA_BINDING_DYNAMIC_CAST_H