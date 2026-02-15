//
// Created by sexey on 14.02.2026.
//
module;
#include <d3d11.h>

#include "hook/hook_invoker.h"

export module hook.impl.resizebuffers;

namespace hook
{
    export HRESULT __attribute__((__fastcall__)) hkResizeBuffers(IDXGISwapChain* instance, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags)
    {
        return MH_CALL_ORIGINAL(hkResizeBuffers)(instance, buffer_count, width, height, new_format, swap_chain_flags);
    }

}