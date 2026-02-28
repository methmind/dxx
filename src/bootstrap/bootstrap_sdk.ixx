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

import sdk.matrices_system;

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

    export bool InitializeSDK(std::unique_ptr<C_ServiceContainer>& services)
    {
        if (!InitializeEngineSDK()) {
            dbg("Unable to initialize engine SDK!");
            return false;
        }

        if (!InitializeFileSystemSDK()) {
            dbg("Unable to initialize filesystem SDK!");
            return false;
        }

        services->add<sdk::C_MatricesSystem>();
        return true;
    }
}
