//
// Created by sexey on 03.03.2026.
//
module;
#include <d3d11.h>
#include <memory>

export module renderer.d3d_texture;

namespace render
{
    export using d3d_texture_t = ID3D11ShaderResourceView;

    export using d3d_texture_ptr_t = std::shared_ptr<d3d_texture_t>;

    struct d3d_texture_destructor_s
    {
        void operator()(d3d_texture_t* srv) const
        {
            if (srv) {
                srv->Release();
            }
        }
    };

    export [[nodiscard]] d3d_texture_ptr_t makeTexture(d3d_texture_t* srv)
    {
        return d3d_texture_ptr_t(srv, d3d_texture_destructor_s{});
    }
}

