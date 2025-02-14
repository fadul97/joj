#ifndef _JOJ_SCENE_OBJECT_3D_H
#define _JOJ_SCENE_OBJECT_3D_H

#include "core/defines.h"

#include "resources/internal_mesh.h"
#include "renderer/d3d11/vertex_buffer_d3d11.h"
#include "renderer/d3d11/index_buffer_d3d11.h"
#include "renderer/renderer.h"
#include "math/jmath.h"
#include "math/jvector3.h"

/*
cbuffer ConstantBuffer : register(b0)
{
    float4x4 wvp;
    float4x4 worldMatrix;
    float4x4 viewMatrix;
    float4x4 projectionMatrix;
};
*/


namespace joj
{
    class JAPI SceneObject3D
    {
    public:
        SceneObject3D();
        ~SceneObject3D();

        void setup(const InternalMesh& mesh, IRenderer* renderer);
        void bind(IRenderer* renderer);

        void translate(const f32 dx, const f32 dy, const f32 dz);
        void set_position(const JVector3 v);

        JVector3 get_position() const;
        u32 get_total_indices() const;

    private:
        D3D11VertexBuffer m_vb;
        D3D11IndexBuffer m_ib;

        JVector3 m_pos;
        u32 m_total_indices;
    };
}

#endif // _JOJ_SCENE_OBJECT_3D_H