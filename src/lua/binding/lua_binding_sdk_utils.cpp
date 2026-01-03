//
// Created by sexey on 07.12.2025.
//

#include "lua_binding_sdk_utils.h"

#include "sdk/util/sdk_color_t.h"

namespace lua::binding
{
    bool C_LuaBindingSdkUtils::apply(const std::weak_ptr<C_ILuaGuardedState>& guardedState)
    {
        const auto tmp = guardedState.lock()->getLuaState();
        auto& luaState = *tmp;

        luaState->new_usertype<sdk::util::color_t>(
            "sColor", sol::factories(
                [] {
                    return sdk::util::color_t{0, 0, 0, 255};
                },
                [](uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
                    return sdk::util::color_t{r, g, b, a};
                }
            ),
            "r", sol::property(
                [](const sdk::util::color_t& c) { return c[0]; },
                [](sdk::util::color_t& c, uint8_t v) { c[0] = v; }
            ),
            "g", sol::property(
                [](const sdk::util::color_t& c) { return c[1]; },
                [](sdk::util::color_t& c, uint8_t v) { c[1] = v; }
            ),
            "b", sol::property(
                [](const sdk::util::color_t& c) { return c[2]; },
                [](sdk::util::color_t& c, uint8_t v) { c[2] = v; }
            ),
            "a", sol::property(
                [](const sdk::util::color_t& c) { return c[3]; },
                [](sdk::util::color_t& c, uint8_t v) { c[3] = v; }
            )
        );

        return true;
    }
} // lua