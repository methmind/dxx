//
// Created by sexey on 10.12.2025.
//

#ifndef DXX_DLC_SDK_INPUT_CONTROLLER_H
#define DXX_DLC_SDK_INPUT_CONTROLLER_H

namespace sdk::custom
{
    class C_InputController
    {
    private:
        static void OnCreateMove();
    public:
        static bool Initialize();
    };
} // sdk

#endif //DXX_DLC_SDK_INPUT_CONTROLLER_H