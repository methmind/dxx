//
// Created by sexey on 21.11.2025.
//

#ifndef DXX_DLC_SDK_HANDLE_INTERFACE_H
#define DXX_DLC_SDK_HANDLE_INTERFACE_H

namespace sdk::iface
{
    template <typename T>
    struct C_IHandle
    {
        T* instance;
    };
}

#endif //DXX_DLC_SDK_HANDLE_INTERFACE_H