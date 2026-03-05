//
// Created by sexey on 14.02.2026.
//
module;
#include <memory>

#include "debug/debug_output.h"

export module bootstrap.sdk;

import service.container;
import service.locator;

import sdk.locator.source2_client;
import sdk.engine.source2_client;

import sdk.locator.source2_engine_to_client;
import sdk.source2_engine_to_client;

import sdk.locator.vfile_system;
import sdk.base_filesystem;

import sdk.locator.game_entity_system;
import sdk.game_entity_system;

import sdk.locator.schema_system;
import sdk.schema_system;

import sdk.matrices_system;

import sdk.entity_list;

namespace bootstrap
{
    bool InitializeEngineSDK()
    {
        const auto source2Client = sdk::GetSource2Client();
        if (!source2Client) {
            dbg("Unable to find C_Source2Client singleton!");
            return false;
        }

        const auto source2EngineToClient = sdk::GetSource2EngineToClient();
        if (!source2EngineToClient) {
            dbg("Unable to find C_Source2EngineToClient singleton!");
            return false;
        }

        C_ServiceLocator::Register<sdk::C_Source2Client>(static_cast<sdk::C_Source2Client*>(source2Client));
        C_ServiceLocator::Register<sdk::C_Source2EngineToClient>(static_cast<sdk::C_Source2EngineToClient*>(source2EngineToClient));

        return true;
    }

    bool InitializeSchemaSDK(const std::unique_ptr<C_ServiceContainer>& services)
    {
        const auto schemaSystem = sdk::GetSchemaSystem();
        if (!schemaSystem) {
            dbg("Unable to find C_SchemaSystem singleton!");
            return false;
        }

        const auto instance = services->add<sdk::C_SchemaSystem>(schemaSystem);
        C_ServiceLocator::Register<sdk::C_SchemaSystem>(instance.get());
        return true;
    }

    bool InitializeFileSystemSDK()
    {
        const auto fileSystem = sdk::GetVFileSystem();
        if (!fileSystem) {
            dbg("Unable to find C_VFileSystem singleton!");
            return false;
        }

        C_ServiceLocator::Register<sdk::C_BaseFileSystem>(static_cast<sdk::C_BaseFileSystem*>(fileSystem));
        return true;
    }

    bool InitializeEntitySDK(const std::unique_ptr<C_ServiceContainer>& services)
    {
        const auto gameEntitySystem = sdk::GetGameEntitySystem();
        if (!gameEntitySystem) {
            dbg("Unable to find C_GameEntitySystem instance!");
            return false;
        }

        C_ServiceLocator::Register<sdk::C_GameEntitySystem>(static_cast<sdk::C_GameEntitySystem*>(gameEntitySystem));
        services->add<sdk::C_EntityList>();

        return true;
    }

    export bool InitializeSDK(const std::unique_ptr<C_ServiceContainer>& services)
    {
        if (!InitializeEngineSDK()) {
            dbg("Unable to initialize engine SDK!");
            return false;
        }

        if (!InitializeSchemaSDK(services)) {
            dbg("Unable to initialize schema SDK!");
            return false;
        }

        if (!InitializeFileSystemSDK()) {
            dbg("Unable to initialize filesystem SDK!");
            return false;
        }

        if (!InitializeEntitySDK(services)) {
            dbg("Unable to initialize entity SDK!");
            return false;
        }

        services->add<sdk::C_MatricesSystem>();
        return true;
    }
}
