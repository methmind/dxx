//
// Created by sexey on 17.11.2025.
//

#include "application_builder.h"
#include "sdk/custom/sdk_entity_list.h"
#include "sdk/singleton/sdk_dota_view_render.h"
#include "sdk/singleton/sdk_game_entity_system.h"
#include "sdk/singleton/sdk_schema_system.h"
#include "sdk/singleton/sdk_source2_client.h"
#include "sdk/singleton/sdk_source2_engine_to_client.h"
#include "service_locator/service_locator.h"

REGISTER_GLOBAL_SERVICE(sdk::singleton::C_SchemaSystem);
REGISTER_GLOBAL_SERVICE(sdk::singleton::C_DotaViewRender);
REGISTER_GLOBAL_SERVICE(sdk::singleton::C_Source2Client);
REGISTER_GLOBAL_SERVICE(sdk::singleton::C_GameEntitySystem);
REGISTER_GLOBAL_SERVICE(sdk::singleton::C_Source2EngineToClient);

namespace bootstrap
{
    bool C_ApplicationBuilder::InitializeSdkStuff(const std::shared_ptr<C_ServiceContainer>& container)
    {
        if (!C_ServiceLocator::getInstance<sdk::singleton::C_SchemaSystem>()->initialize()) {
            dbg("Unable to initialize sdk::C_ShemaSystem");
            return false;
        }

        if (!C_ServiceLocator::getInstance<sdk::singleton::C_DotaViewRender>()->initialize()) {
            dbg("Unable to initialize sdk::C_DotaViewRender");
            return false;
        }

        if (!C_ServiceLocator::getInstance<sdk::singleton::C_Source2Client>()->initialize()) {
            dbg("Unable to initialize sdk::C_Source2Client");
            return false;
        }

        if (!C_ServiceLocator::getInstance<sdk::singleton::C_GameEntitySystem>()->initialize()) {
            dbg("Unable to initialize sdk::C_GameEntitySystem");
            return false;
        }

        if (!C_ServiceLocator::getInstance<sdk::singleton::C_Source2EngineToClient>()->initialize()) {
            dbg("Unable to initialize sdk::C_Source2EngineToClient");
            return false;
        }

        if (!container->add<sdk::custom::C_EntityList>()->initialize()) {
            dbg("Unable to initialize sdk::custom::C_EntityList!");
            return false;
        }

        return true;
    }
}
