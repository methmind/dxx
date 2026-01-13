#pragma once

#include <string_view>

namespace memory {
    class OffsetManager {
    public:
        static void Initialize();

        static void* GetOnLevelInit();
        static void* GetOnLevelShutdown();
        static void* GetScreenTransform();
        static void* GetGameSystemInitAll();
        static void* GetGameSystemFactoryMovOpcode();
        static void* GetBaseModelEntityChangeColor();
        static void* GetCInputCreateMove();
        static void* GetCInputGetUserCmd();
        static void* GetGetScreenPlayerController();
        static void* GetGetPlayerScreenId();
        static void* GetGetScreenCmdBuffer();
        static void* GetMovRcxGlobalCmdCircularBuffer();
        static void* GetAddRaxSequenceNumberOffset();
        static void* GetViewRenderInstance();

    private:
        struct Offsets {
            void* OnLevelInit = nullptr;
            void* OnLevelShutdown = nullptr;
            void* ScreenTransform = nullptr;
            void* GameSystemInitAll = nullptr;
            void* GameSystemFactoryMovOpcode = nullptr;
            void* BaseModelEntityChangeColor = nullptr;
            void* CInputCreateMove = nullptr;
            void* CInputGetUserCmd = nullptr;
            void* GetScreenPlayerController = nullptr;
            void* GetPlayerScreenId = nullptr;
            void* GetScreenCmdBuffer = nullptr;
            void* MovRcxGlobalCmdCircularBuffer = nullptr;
            void* AddRaxSequenceNumberOffset = nullptr;
            void* GetViewRenderInstance = nullptr;
        };

        static Offsets offsets_;
        static bool initialized_;
    };
}
