//
// Created by sexey on 17.11.2025.
//

#include "application_builder.h"
#include "sdk/custom/sdk_entity_list.h"
#include "sdk/custom/sdk_input_controller.h"
#include "sdk/custom/sdk_matrices_system.h"
#include "sdk/singleton/sdk_base_game_system_factory.h"
#include "sdk/singleton/sdk_dota_camera_manager.h"
#include "sdk/singleton/sdk_dota_input.h"
#include "sdk/singleton/sdk_dota_view_render.h"
#include "sdk/singleton/sdk_game_entity_system.h"
#include "sdk/singleton/sdk_render_game_system.h"
#include "sdk/singleton/sdk_schema_system.h"
#include "sdk/singleton/sdk_source2_client.h"
#include "sdk/singleton/sdk_source2_engine_to_client.h"
#include "service_locator/service_locator.h"

REGISTER_GLOBAL_SERVICE(sdk::singleton::C_SchemaSystem);
REGISTER_GLOBAL_SERVICE(sdk::singleton::C_DotaViewRender);
REGISTER_GLOBAL_SERVICE(sdk::singleton::C_Source2Client);
REGISTER_GLOBAL_SERVICE(sdk::singleton::C_GameEntitySystem);
REGISTER_GLOBAL_SERVICE(sdk::singleton::C_Source2EngineToClient);
REGISTER_GLOBAL_SERVICE(sdk::singleton::C_BaseGameSystemFactory);
REGISTER_GLOBAL_SERVICE(sdk::singleton::C_DotaInput);
REGISTER_GLOBAL_SERVICE(sdk::singleton::C_DotaCamaraManager);

namespace bootstrap
{
    bool C_ApplicationBuilder::InitializeSdkStuff(const std::shared_ptr<C_ServiceContainer>& container)
    {
        if (!C_ServiceLocator::getInstance<sdk::singleton::C_SchemaSystem>()->initialize()) {
            dbg("Unable to initialize sdk::singleton::C_ShemaSystem");
            return false;
        }

        if (!C_ServiceLocator::getInstance<sdk::singleton::C_BaseGameSystemFactory>()->initialize()) {
            dbg("Unable to initialize sdk::singleton::C_BaseGameSystemFactory");
            return false;
        }

        if (!C_ServiceLocator::getInstance<sdk::singleton::C_DotaViewRender>()->initialize()) {
            dbg("Unable to initialize sdk::singleton::C_DotaViewRender");
            return false;
        }

        if (!C_ServiceLocator::getInstance<sdk::singleton::C_Source2Client>()->initialize()) {
            dbg("Unable to initialize sdk::singleton::C_Source2Client");
            return false;
        }

        if (!C_ServiceLocator::getInstance<sdk::singleton::C_GameEntitySystem>()->initialize()) {
            dbg("Unable to initialize sdk::singleton::C_GameEntitySystem");
            return false;
        }

        if (!C_ServiceLocator::getInstance<sdk::singleton::C_Source2EngineToClient>()->initialize()) {
            dbg("Unable to initialize sdk::singleton::C_Source2EngineToClient");
            return false;
        }

        if (!C_ServiceLocator::getInstance<sdk::singleton::C_RenderGameSystem>()->initialize()) {
            dbg("Unable to initialize sdk::singleton::C_RenderGameSystem");
            return false;
        }

        if (!C_ServiceLocator::getInstance<sdk::singleton::C_DotaInput>()->initialize()) {
            dbg("Unable to initialize sdk::singleton::C_DotaInput");
            return false;
        }

        if (!C_ServiceLocator::getInstance<sdk::singleton::C_DotaCamaraManager>()->initialize()) {
            dbg("Unable to initialize sdk::singleton::C_DotaCamaraManager");
            return false;
        }

        if (!sdk::custom::C_InputController::Initialize()) [[unlikely]] {
            dbg("Unable to initialize sdk::singleton::C_InputController!");
            return false;
        }

        if (!container->add<sdk::custom::C_EntityList>()->initialize()) {
            dbg("Unable to initialize sdk::custom::C_EntityList!");
            return false;
        }

        if (!container->add<sdk::custom::C_MatricesSystem>()->initialize()) {
            dbg("Unable to initialize sdk::custom::C_MatricesSystem");
            return false;
        }

        return true;
    }
}
