//
// Created by sexey on 01.12.2025.
//

#ifndef DXX_DLC_LUA_BINDING_SDK_ENTITIES_H
#define DXX_DLC_LUA_BINDING_SDK_ENTITIES_H

#include <utility>

#include "lua_binding_interface.h"
#include "hash/xxhash_wrapper.h"
#include "sdk/custom/sdk_entity_list.h"

namespace lua::binding

{
    constexpr auto ENTITIES_NAMESPACE_NAME = "entities";

    class C_LuaEntityListView
    {
    private:
        const std::vector<sdk::iface::C_EntityInstance*>* list_;
        std::function<sol::object(sol::state_view&, sdk::iface::C_EntityInstance*)> caster_;
        mutable sol::state_view lua_;

    public:

        C_LuaEntityListView(sol::state_view lua, const std::vector<sdk::iface::C_EntityInstance*>& list, decltype(caster_) caster)
            : list_(&list), caster_(std::move(caster)), lua_(std::move(lua)) {}

        [[nodiscard]] size_t size() const
        {
            return this->list_ ? this->list_->size() : 0;
        }

        [[nodiscard]] sol::object get(const size_t index) const
        {
            if (!this->list_ || index < 1 || index > this->list_->size()) {
                return sol::make_object(this->lua_, sol::lua_nil);
            }

            return this->caster_(this->lua_, (*this->list_)[index - 1]);
        }
    };

    class C_LuaBindingSdkEntities final : public C_ILuaBinding
    {
    private:
        using caster_func_t = std::function<sol::object(sol::state_view&, sdk::iface::C_EntityInstance*)>;

        std::unordered_map<std::string, caster_func_t, xx_hashier_s, std::equal_to<>> typeCasters_;
        std::shared_ptr<sdk::custom::C_EntityList> entities_;

        template<typename T>
        void registerTypeCaster(const std::string& name)
        {
            this->typeCasters_[name] = [](sol::state_view& lua, sdk::iface::C_EntityInstance* ptr) -> sol::object {
                return sol::make_object(lua, static_cast<T*>(ptr));
            };
        }

        void registerEntities(sol::state& state);

        void registerCaster();

    public:

        bool apply(const std::weak_ptr<C_ILuaGuardedState>& guardedState) override;

        explicit C_LuaBindingSdkEntities(const std::shared_ptr<sdk::custom::C_EntityList>& entities) : entities_(entities) {}

        ~C_LuaBindingSdkEntities() override = default;
    };
} // lua

#endif //DXX_DLC_LUA_BINDING_SDK_ENTITIES_H
