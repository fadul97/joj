#ifndef _JOJ_D3D11_SCENE_H
#define _JOJ_D3D11_SCENE_H

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
    class JOJ_API D3D11Scene : public Scene
    {
    public:
        D3D11Scene();
        ~D3D11Scene();

        void init(const GraphicsDevice& device, Camera& camera) override;
        void update(const f32 dt) override;
        void draw(IRenderer& renderer) override;
        void shutdown() override;

        void draw_collisions(IRenderer& renderer) override;

    protected:
        D3D11VertexBuffer m_sprite_vertex_buffer2D;
        D3D11IndexBuffer m_sprite_index_buffer2D;
        D3D11ConstantBuffer m_sprite_constant_buffer;
        D3D11Shader m_sprite_shader;
        D3D11InputLayout m_sprite_layout;

        joj::D3D11Shader m_collision_shader;
        joj::D3D11InputLayout m_collision_input_layout;
        joj::D3D11VertexBuffer m_collision_vertex_buffer;
        joj::D3D11ConstantBuffer m_collision_constant_buffer;
    };
}

#endif // JOJ_PLATFORM_WINDOWS

#endif // _JOJ_D3D11_SCENE_H