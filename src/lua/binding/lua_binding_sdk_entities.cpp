//
// Created by sexey on 01.12.2025.
//

#include "lua_binding_sdk_entities.h"

#include "debug/debug_output.h"
#include "sdk/interface/sdk_base_entity.h"
#include "sdk/interface/sdk_base_model_entity.h"
#include "sdk/interface/sdk_dota_base_npc.h"
#include "sdk/interface/sdk_dota_player_controller.h"
#include "sdk/singleton/sdk_game_entity_system.h"
#include "sdk/singleton/sdk_source2_engine_to_client.h"

namespace lua::binding
{
    void C_LuaBindingSdkEntities::registerEntities(sol::state& state)
    {
        auto luaBaseEntityHandle = state.new_usertype<sdk::util::C_BaseEntityHandle>(
            "C_BaseEntityHandle", sol::no_constructor
        );
        luaBaseEntityHandle.set_function("is_valid", &sdk::util::C_BaseEntityHandle::isValid);
        luaBaseEntityHandle.set_function("get_entry_index", &sdk::util::C_BaseEntityHandle::getEntryIndex);

        auto luaEntityListView = state.new_usertype<C_LuaEntityListView>("C_LuaEntityListView", sol::no_constructor);
        luaEntityListView.set_function(sol::meta_function::length, &C_LuaEntityListView::size);
        luaEntityListView.set_function(sol::meta_function::index, &C_LuaEntityListView::get);

        auto luaEntityIdentity = state.new_usertype<sdk::iface::C_EntityIdentity>(
            "C_EntityIdentity", sol::no_constructor
        );
        luaEntityIdentity.set_function("get_index", &sdk::iface::C_EntityIdentity::getEntityHandle);
        luaEntityIdentity.set_function("get_name", &sdk::iface::C_EntityIdentity::getName);
        luaEntityIdentity.set_function("get_designer_name", &sdk::iface::C_EntityIdentity::getDesignerName);
        luaEntityIdentity.set_function("get_flags", &sdk::iface::C_EntityIdentity::getFlags);

        auto luaEntityInstance = state.new_usertype<sdk::iface::C_EntityInstance>("C_EntityInstance", sol::no_constructor);
        luaEntityInstance.set_function("get_class_info", &sdk::iface::C_EntityInstance::getClassInfo);
        luaEntityInstance.set_function("get_identity", &sdk::iface::C_EntityInstance::getIdentity);
        luaEntityInstance.set_function("as",
            [this](sol::this_state state, sdk::iface::C_EntityInstance* self, const std::string& className) {
                sol::state_view lua(state);

                const auto it = this->typeCasters_.find(className);
                if (it == this->typeCasters_.end()) {
                    luaL_error(state.lua_state(), "Unable to cast entity!");
                    __builtin_unreachable();
                }

                auto casted = it->second(lua, self);
                if (!casted.valid()) {
                    luaL_error(state.lua_state(), "Unable to cast entity!");
                    __builtin_unreachable();
                }

                return casted;
            }
        );

        auto luaBaseEntity = state.new_usertype<sdk::iface::C_BaseEntity>(
            "C_BaseEntity", sol::no_constructor,
            sol::base_classes, sol::bases<sdk::iface::C_EntityInstance>()
        );
        luaBaseEntity.set_function("get_max_health", &sdk::iface::C_BaseEntity::getMaxHealth);
        luaBaseEntity.set_function("get_health", &sdk::iface::C_BaseEntity::getHealth);
        luaBaseEntity.set_function("get_flags", &sdk::iface::C_BaseEntity::getEntityFlags);
        luaBaseEntity.set_function("get_team", &sdk::iface::C_BaseEntity::getTeam);

        auto luaPlayerController = state.new_usertype<sdk::iface::C_DotaPlayerController>(
            "C_DotaPlayerController", sol::no_constructor,
            sol::base_classes, sol::bases<sdk::iface::C_BaseEntity, sdk::iface::C_EntityInstance>()
        );
        luaPlayerController.set_function("get_player_name", &sdk::iface::C_DotaPlayerController::getPlayerName);
        luaPlayerController.set_function("get_steam_id", &sdk::iface::C_DotaPlayerController::getSteamID);
        luaPlayerController.set_function("get_assigned_hero_handle", &sdk::iface::C_DotaPlayerController::getAssignedHeroHandle);

        auto luaBaseModelEntity = state.new_usertype<sdk::iface::C_BaseModelEntity>(
            "C_BaseModelEntity", sol::no_constructor,
            sol::base_classes, sol::bases<sdk::iface::C_BaseEntity, sdk::iface::C_EntityInstance>()
        );
        luaBaseModelEntity.set_function("get_model_color", &sdk::iface::C_BaseModelEntity::getRenderColor);
        luaBaseModelEntity.set_function("set_model_color", &sdk::iface::C_BaseModelEntity::changeModelColor);

        auto luaBaseNpc = state.new_usertype<sdk::iface::C_DotaBaseNPC>(
            "C_DOTA_BaseNPC", sol::no_constructor,
            sol::base_classes, sol::bases<sdk::iface::C_BaseModelEntity, sdk::iface::C_BaseEntity, sdk::iface::C_EntityInstance>()
        );
        luaBaseNpc.set_function("is_clone", &sdk::iface::C_DotaBaseNPC::isClone);
        luaBaseNpc.set_function("get_modifier_manager", &sdk::iface::C_DotaBaseNPC::getModifierManager);
        luaBaseNpc.set_function("get_unit_name", &sdk::iface::C_DotaBaseNPC::getUnitName);
        luaBaseNpc.set_function("mark_as_illusion",
            [](sdk::iface::C_DotaBaseNPC* self, const sdk::util::color_t& color) {
                self->isSeenAsIllusion() = true;
                self->changeModelColor(color);
            }
        );
        luaBaseNpc.set_function("reset_illusion_state",
            [](sdk::iface::C_DotaBaseNPC* self) {
                self->isSeenAsIllusion() = false;
                self->changeModelColor(sdk::util::color_t{255, 255, 255, 255});
            }
        );

        auto luaBaseNpcHero = state.new_usertype<sdk::iface::C_DotaBaseNPC_Hero>(
            "C_DOTA_BaseNPC_Hero", sol::no_constructor,
            sol::base_classes, sol::bases<sdk::iface::C_DotaBaseNPC, sdk::iface::C_BaseModelEntity, sdk::iface::C_BaseEntity, sdk::iface::C_EntityInstance>()
        );
        luaBaseNpcHero.set_function("get_replicating_hero_handle", &sdk::iface::C_DotaBaseNPC_Hero::getReplicatingHeroHandle);
    }

    void C_LuaBindingSdkEntities::registerCaster()
    {
        registerTypeCaster<sdk::iface::C_EntityInstance>("CEntityInstance");
        registerTypeCaster<sdk::iface::C_BaseEntity>("C_BaseEntity");
        registerTypeCaster<sdk::iface::C_DotaPlayerController>("C_DOTAPlayerController");
        registerTypeCaster<sdk::iface::C_BaseModelEntity>("C_BaseModelEntity");
        registerTypeCaster<sdk::iface::C_DotaBaseNPC>("C_DOTA_BaseNPC");
        registerTypeCaster<sdk::iface::C_DotaBaseNPC_Hero>("C_DOTA_BaseNPC_Hero");
    }

    bool C_LuaBindingSdkEntities::apply(const std::weak_ptr<C_ILuaGuardedState>& guardedState)
    {
        const auto tmp = guardedState.lock()->getLuaState();
        auto& luaState = *tmp;

        auto entitiesNamespace = luaState[ENTITIES_NAMESPACE_NAME].get_or_create<sol::table>();
        if (!entitiesNamespace.valid()) {
            dbg("Unable to create entities namespace!");
            return false;
        }

        registerEntities(luaState);
        registerCaster();

        entitiesNamespace.set_function("get_local_controller", []() -> sdk::iface::C_DotaPlayerController* {
            const auto localPlayerID = C_ServiceLocator::getInstance<sdk::singleton::C_Source2EngineToClient>()->getLocalPlayerID();
            if (!localPlayerID) {
                return nullptr;
            }

            return C_ServiceLocator::getInstance<sdk::singleton::C_GameEntitySystem>()->
                getBaseEntity<sdk::iface::C_DotaPlayerController>(localPlayerID);
        });

        entitiesNamespace.set_function("find_by_handle",
            [](const sdk::util::C_BaseEntityHandle entityHandle) {
                return C_ServiceLocator::getInstance<sdk::singleton::C_GameEntitySystem>()->
                    getBaseEntity<sdk::iface::C_EntityInstance>(entityHandle.getEntryIndex());
            }
        );

        entitiesNamespace.set_function("count", [this](const std::string& className) {
            return this->entities_->count(className);
        });

        entitiesNamespace.set_function("find", [this](const std::string& className) -> auto& {
            return this->entities_->find(className);
        });

        entitiesNamespace.set_function("find_as",
            [this](sol::this_state state, const std::string& className, const std::string& castTo) {
                const auto& list = this->entities_->find(className);

                const auto caster = this->typeCasters_.find(castTo);
                if (caster == this->typeCasters_.end()) {
                    luaL_error(state.lua_state(), "Unable to get lua container!");
                    __builtin_unreachable();
                }

                return sol::make_object(state, C_LuaEntityListView(state, list, caster->second));
            }
        );

        return true;
    }
} // lua