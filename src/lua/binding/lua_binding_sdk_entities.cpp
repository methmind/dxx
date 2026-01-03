//
// Created by sexey on 01.12.2025.
//

#include "lua_binding_sdk_entities.h"

#include "debug/debug_output.h"
#include "sdk/datatype/sdk_base_entity.h"
#include "sdk/datatype/sdk_base_model_entity.h"
#include "sdk/datatype/sdk_dota_base_npc.h"
#include "sdk/datatype/sdk_dota_player_controller.h"
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

        auto luaEntityIdentity = state.new_usertype<sdk::datatype::C_EntityIdentity>(
            "C_EntityIdentity", sol::no_constructor
        );
        luaEntityIdentity.set_function("get_index", &sdk::datatype::C_EntityIdentity::getEntityHandle);
        luaEntityIdentity.set_function("get_name", &sdk::datatype::C_EntityIdentity::getName);
        luaEntityIdentity.set_function("get_designer_name", &sdk::datatype::C_EntityIdentity::getDesignerName);
        luaEntityIdentity.set_function("get_flags", &sdk::datatype::C_EntityIdentity::getFlags);

        auto luaEntityInstance = state.new_usertype<sdk::datatype::C_EntityInstance>("C_EntityInstance", sol::no_constructor);
        luaEntityInstance.set_function("get_class_info", &sdk::datatype::C_EntityInstance::getClassInfo);
        luaEntityInstance.set_function("get_identity", &sdk::datatype::C_EntityInstance::getIdentity);
        luaEntityInstance.set_function("as",
            [this](sol::this_state state, sdk::datatype::C_EntityInstance* self, const std::string_view& className) {
                return this->caster_.cast(sol::state_view(state), self, className);
            }
        );

        auto luaBaseEntity = state.new_usertype<sdk::datatype::C_BaseEntity>(
            "C_BaseEntity", sol::no_constructor,
            sol::base_classes, sol::bases<sdk::datatype::C_EntityInstance>()
        );
        luaBaseEntity.set_function("get_max_health", &sdk::datatype::C_BaseEntity::getMaxHealth);
        luaBaseEntity.set_function("get_health", &sdk::datatype::C_BaseEntity::getHealth);
        luaBaseEntity.set_function("get_flags", &sdk::datatype::C_BaseEntity::getEntityFlags);
        luaBaseEntity.set_function("get_team", &sdk::datatype::C_BaseEntity::getTeam);

        auto luaPlayerController = state.new_usertype<sdk::datatype::C_DotaPlayerController>(
            "C_DotaPlayerController", sol::no_constructor,
            sol::base_classes, sol::bases<sdk::datatype::C_BaseEntity, sdk::datatype::C_EntityInstance>()
        );
        luaPlayerController.set_function("get_player_name", &sdk::datatype::C_DotaPlayerController::getPlayerName);
        luaPlayerController.set_function("get_steam_id", &sdk::datatype::C_DotaPlayerController::getSteamID);
        luaPlayerController.set_function("get_assigned_hero_handle", &sdk::datatype::C_DotaPlayerController::getAssignedHeroHandle);

        auto luaBaseModelEntity = state.new_usertype<sdk::datatype::C_BaseModelEntity>(
            "C_BaseModelEntity", sol::no_constructor,
            sol::base_classes, sol::bases<sdk::datatype::C_BaseEntity, sdk::datatype::C_EntityInstance>()
        );
        luaBaseModelEntity.set_function("get_model_color", &sdk::datatype::C_BaseModelEntity::getRenderColor);
        luaBaseModelEntity.set_function("set_model_color", &sdk::datatype::C_BaseModelEntity::changeModelColor);

        auto luaBaseNpc = state.new_usertype<sdk::datatype::C_DotaBaseNPC>(
            "C_DOTA_BaseNPC", sol::no_constructor,
            sol::base_classes, sol::bases<sdk::datatype::C_BaseModelEntity, sdk::datatype::C_BaseEntity, sdk::datatype::C_EntityInstance>()
        );
        luaBaseNpc.set_function("is_clone", &sdk::datatype::C_DotaBaseNPC::isClone);
        luaBaseNpc.set_function("get_modifier_manager", &sdk::datatype::C_DotaBaseNPC::getModifierManager);
        luaBaseNpc.set_function("get_unit_name", &sdk::datatype::C_DotaBaseNPC::getUnitName);
        luaBaseNpc.set_function("mark_as_illusion",
            [](sdk::datatype::C_DotaBaseNPC* self, const sdk::util::color_t& color) {
                self->isSeenAsIllusion() = true;
                self->changeModelColor(color);
            }
        );
        luaBaseNpc.set_function("reset_illusion_state",
            [](sdk::datatype::C_DotaBaseNPC* self) {
                self->isSeenAsIllusion() = false;
                self->changeModelColor(sdk::util::color_t{255, 255, 255, 255});
            }
        );

        auto luaBaseNpcHero = state.new_usertype<sdk::datatype::C_DotaBaseNPC_Hero>(
            "C_DOTA_BaseNPC_Hero", sol::no_constructor,
            sol::base_classes, sol::bases<sdk::datatype::C_DotaBaseNPC, sdk::datatype::C_BaseModelEntity, sdk::datatype::C_BaseEntity, sdk::datatype::C_EntityInstance>()
        );
        luaBaseNpcHero.set_function("get_replicating_hero_handle", &sdk::datatype::C_DotaBaseNPC_Hero::getReplicatingHeroHandle);
    }

    void C_LuaBindingSdkEntities::registerCaster()
    {
        this->caster_.add<sdk::datatype::C_EntityInstance>("CEntityInstance");
        this->caster_.add<sdk::datatype::C_BaseEntity>("C_BaseEntity");
        this->caster_.add<sdk::datatype::C_DotaPlayerController>("C_DOTAPlayerController");
        this->caster_.add<sdk::datatype::C_BaseModelEntity>("C_BaseModelEntity");
        this->caster_.add<sdk::datatype::C_DotaBaseNPC>("C_DOTA_BaseNPC");
        this->caster_.add<sdk::datatype::C_DotaBaseNPC_Hero>("C_DOTA_BaseNPC_Hero");
    }

    bool C_LuaBindingSdkEntities::apply(const std::weak_ptr<C_ILuaGuardedState>& guardedState)
    {
        const auto tmp = guardedState.lock()->getLuaState();
        const auto& luaState = *tmp;

        auto entitiesNamespace = luaState->create_named_table(ENTITIES_NAMESPACE_NAME);
        if (!entitiesNamespace.valid()) {
            dbg("Unable to create entities namespace!");
            return false;
        }

        registerEntities(*luaState);
        registerCaster();

        entitiesNamespace.set_function("get_local_controller", []() -> sdk::datatype::C_DotaPlayerController* {
            const auto localPlayerID = C_ServiceLocator::getInstance<sdk::singleton::C_Source2EngineToClient>()->getLocalPlayerID();
            if (!localPlayerID) {
                return nullptr;
            }

            return C_ServiceLocator::getInstance<sdk::singleton::C_GameEntitySystem>()->
                getBaseEntity<sdk::datatype::C_DotaPlayerController>(localPlayerID);
        });

        entitiesNamespace.set_function("find_by_handle",
            [](const sdk::util::C_BaseEntityHandle entityHandle) {
                return C_ServiceLocator::getInstance<sdk::singleton::C_GameEntitySystem>()->
                    getBaseEntity<sdk::datatype::C_EntityInstance>(entityHandle.getEntryIndex());
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

                const auto caster = this->caster_.find(castTo);
                if (!caster) {
                    luaL_error(state.lua_state(), "Unable to get lua container!");
                    __builtin_unreachable();
                }

                return sol::make_object(state, C_LuaEntityListView(state, list, caster));
            }
        );

        return true;
    }
} // lua