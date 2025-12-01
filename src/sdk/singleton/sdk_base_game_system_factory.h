//
// Created by sexey on 01.12.2025.
//

#ifndef DXX_DLC_SDK_BASE_GAME_SYSTEM_FACTORY_H
#define DXX_DLC_SDK_BASE_GAME_SYSTEM_FACTORY_H

#include <string_view>

namespace sdk::singleton
{
    class C_IGameSystemFactory
    {
    private:
        void* vtable_{};
    public:
        C_IGameSystemFactory* next{};
        const char* name{};
        void* gameSystem{};
    };

    class C_BaseGameSystemFactory
    {
    private:
        void* instance_;

    public:

        [[nodiscard]] void* find(const std::string_view& name) const;

        bool initialize();

        C_BaseGameSystemFactory() = default;

        ~C_BaseGameSystemFactory() = default;
    };
} // sdk

#endif //DXX_DLC_SDK_BASE_GAME_SYSTEM_FACTORY_H