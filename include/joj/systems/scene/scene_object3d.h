#ifndef _JOJ_SCENE_OBJECT_3D_H
#define _JOJ_SCENE_OBJECT_3D_H
#include "joj/core/defines.h"

#include "joj/resources/internal_mesh.h"
#include "joj/renderer/d3d11/vertex_buffer_d3d11.h"
#include "joj/renderer/d3d11/index_buffer_d3d11.h"
#include "joj/renderer/renderer.h"
#include "joj/math/jmath.h"
#include "joj/math/jvector3.h"

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
    class JOJ_API SceneObject3D
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