//
// Created by sexey on 12.11.2025.
//

#ifndef DXX_DLC_SDK_SOURCE2_CLIENT_H
#define DXX_DLC_SDK_SOURCE2_CLIENT_H

namespace sdk
{
    class C_Source2Client
    {
    private:
        void* instance_;

    public:

        bool initialize();

        C_Source2Client() : instance_(nullptr) {}

        ~C_Source2Client() = default;
    };
} // sdk

#endif //DXX_DLC_SDK_SOURCE2_CLIENT_H