//
// Created by sexey on 07.12.2025.
//

#ifndef DXX_DLC_LUA_BINDING_UTL_VECTOR_H
#define DXX_DLC_LUA_BINDING_UTL_VECTOR_H

#include "sol/sol.hpp"
#include "sdk/memory/sdk_memory_utl_vector.h"

namespace sol
{
    template<typename T>
    struct is_container<sdk::memory::C_UtlVector<T>> : std::true_type {};

    template<typename T>
    struct usertype_container<sdk::memory::C_UtlVector<T>>
    {
        using type = sdk::memory::C_UtlVector<T>;
        using valueType = T;
        using iterator = valueType*;
        using const_iterator = const iterator;

        static size_t size(lua_State* L)
        {
            auto& vec = stack::get<type&>(L, 1);
            return stack::push(L, vec.size());
        }

        static uint32_t push_back(lua_State* L)
        {
            auto& vec = stack::get<type&>(L, 1);
            auto value = stack::get<valueType>(L, 2);
            vec.push_back(std::move(value));

            return 0;
        }

        static uint32_t at(lua_State* L)
        {
            auto& vec = stack::get<type&>(L, 1);
            const size_t index = stack::get<size_t>(L, 2);

            return stack::push(L, vec[index - 1]);
        }

        static int index_get(lua_State* L) {
            if (lua_type(L, 2) == LUA_TNUMBER) {
                return at(L);
            }

            return 0;
        }

        static uint32_t begin(lua_State* L)
        {
            auto& vec = stack::get<type&>(L, 1);
            return stack::push(L, vec.begin());
        }

        static uint32_t end(lua_State* L)
        {
            auto& vec = stack::get<type&>(L, 1);
            return stack::push(L, vec.end());
        }

        static uint32_t clear(lua_State* L)
        {
            auto& vec = stack::get<type&>(L, 1);
            vec.clear();

            return 0;
        }
    };
}

#endif //DXX_DLC_LUA_BINDING_UTL_VECTOR_H