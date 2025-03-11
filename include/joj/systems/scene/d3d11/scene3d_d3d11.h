#ifndef _JOJ_D3D11_SCENE_3D_H
#define _JOJ_D3D11_SCENE_3D_H

#include "core/defines.h"

#if JOJ_PLATFORM_WINDOWS

#include "systems/scene/scene.h"
#include "renderer/d3d11/vertex_buffer_d3d11.h"
#include "renderer/d3d11/index_buffer_d3d11.h"
#include "renderer/d3d11/shader_d3d11.h"
#include "renderer/d3d11/input_layout_d3d11.h"
#include "renderer/d3d11/constant_buffer_d3d11.h"

namespace joj
{
    class JOJ_API D3D11Scene3D : public Scene
    {
    public:
        D3D11Scene3D();
        ~D3D11Scene3D();

        void init(const GraphicsDevice& device, Camera& camera) override;
        void update(const f32 dt) override;
        void draw(IRenderer& renderer) override;
        void shutdown() override;

        void draw_collisions(IRenderer& renderer) override;

    protected:
        D3D11VertexBuffer m_model_vb;
        D3D11IndexBuffer m_model_ib;
        D3D11ConstantBuffer m_model_cb;
        D3D11Shader m_model_shader;
        D3D11InputLayout m_model_layout;
    };
}

#endif // JOJ_PLATFORM_WINDOWS

#endif // _JOJ_D3D11_SCENE_3D_H