#include "offset_manager.h"
#include "pattern_scanner.h"
#include "../sdk/sdk_signature.h"
#include <Windows.h>
#include <cstdio>

namespace memory {

    OffsetManager::Offsets OffsetManager::offsets_;
    bool OffsetManager::initialized_ = false;

    void OffsetManager::Initialize() {
        if (initialized_) return;

        HMODULE clientModule = GetModuleHandleA("client.dll");
        if (!clientModule) {
            // TODO: Better error handling
            printf("[OffsetManager] Failed to get client.dll module handle!\n");
            return;
        }

        auto find = [&](const char* name, const char* sig) -> void* {
             void* addr = memory::FindPattern(clientModule, sig);
             if (!addr) {
                 printf("[OffsetManager] Failed to find pattern: %s\n", name);
             } else {
                 // printf("[OffsetManager] Found %s at %p\n", name, addr);
             }
             return addr;
        };

        offsets_.OnLevelInit = find("OnLevelInit", sdk::signature::ON_LEVEL_INIT_CALLBACK);
        offsets_.OnLevelShutdown = find("OnLevelShutdown", sdk::signature::ON_LEVEL_SHUTDOWN_CALLBACK);
        offsets_.ScreenTransform = find("ScreenTransform", sdk::signature::SCREEN_TRANSFORM_FUNC);
        offsets_.GameSystemInitAll = find("GameSystemInitAll", sdk::signature::GAME_SYSTEM_INIT_ALL_FUNC);
        offsets_.GameSystemFactoryMovOpcode = find("GameSystemFactoryMovOpcode", sdk::signature::GAME_SYSTEM_FACTORY_MOV_OPCODE);
        offsets_.BaseModelEntityChangeColor = find("BaseModelEntityChangeColor", sdk::signature::BASE_MODEL_ENTITY_CHANGE_COLOR_FUNC);
        offsets_.CInputCreateMove = find("CInputCreateMove", sdk::signature::CINPUT_CREATE_MOVE_FUNC);
        offsets_.CInputGetUserCmd = find("CInputGetUserCmd", sdk::signature::CINPUT_GET_USER_CMD_FUNC);
        offsets_.GetScreenPlayerController = find("GetScreenPlayerController", sdk::signature::GET_SCREEN_PLAYER_CONTROLLER_FUNC);
        offsets_.GetPlayerScreenId = find("GetPlayerScreenId", sdk::signature::GET_PLAYER_SCREEN_ID_FUNC);
        offsets_.GetScreenCmdBuffer = find("GetScreenCmdBuffer", sdk::signature::GET_SCREEN_CMD_BUFFER_FUNC);
        offsets_.MovRcxGlobalCmdCircularBuffer = find("MovRcxGlobalCmdCircularBuffer", sdk::signature::MOV_RCX_GLOBAL_CMD_CIRCULAR_BUFFER);
        offsets_.AddRaxSequenceNumberOffset = find("AddRaxSequenceNumberOffset", sdk::signature::ADD_RAX_SEQUENCE_NUMBER_OFFSET);
        offsets_.GetViewRenderInstance = find("GetViewRenderInstance", sdk::signature::GET_VIEW_RENDER_INSTANCE_FUNC);

        initialized_ = true;
    }

    void* OffsetManager::GetOnLevelInit() { return offsets_.OnLevelInit; }
    void* OffsetManager::GetOnLevelShutdown() { return offsets_.OnLevelShutdown; }
    void* OffsetManager::GetScreenTransform() { return offsets_.ScreenTransform; }
    void* OffsetManager::GetGameSystemInitAll() { return offsets_.GameSystemInitAll; }
    void* OffsetManager::GetGameSystemFactoryMovOpcode() { return offsets_.GameSystemFactoryMovOpcode; }
    void* OffsetManager::GetBaseModelEntityChangeColor() { return offsets_.BaseModelEntityChangeColor; }
    void* OffsetManager::GetCInputCreateMove() { return offsets_.CInputCreateMove; }
    void* OffsetManager::GetCInputGetUserCmd() { return offsets_.CInputGetUserCmd; }
    void* OffsetManager::GetGetScreenPlayerController() { return offsets_.GetScreenPlayerController; }
    void* OffsetManager::GetGetPlayerScreenId() { return offsets_.GetPlayerScreenId; }
    void* OffsetManager::GetGetScreenCmdBuffer() { return offsets_.GetScreenCmdBuffer; }
    void* OffsetManager::GetMovRcxGlobalCmdCircularBuffer() { return offsets_.MovRcxGlobalCmdCircularBuffer; }
    void* OffsetManager::GetAddRaxSequenceNumberOffset() { return offsets_.AddRaxSequenceNumberOffset; }
    void* OffsetManager::GetViewRenderInstance() { return offsets_.GetViewRenderInstance; }

}
