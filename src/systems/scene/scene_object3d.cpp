#include "systems/scene/scene_object3d.h"

#include "renderer/d3d11/renderer_d3d11.h"
#include "core/jmacros.h"

joj::SceneObject3D::SceneObject3D()
    : m_total_indices(0)
{
}

joj::SceneObject3D::~SceneObject3D()
{
}

void joj::SceneObject3D::setup(const InternalMesh& mesh, IRenderer* renderer)
{
    m_vb.setup(joj::BufferUsage::Immutable, joj::CPUAccessType::None,
        sizeof(joj::Vertex::PosColorNormal) * mesh.vertices.size(), mesh.vertices.data());
    JOJ_LOG_IF_FAIL(m_vb.create(renderer->get_device()));

    m_ib.setup(sizeof(u32) * mesh.indices.size(), mesh.indices.data());
    JOJ_LOG_IF_FAIL(m_ib.create(renderer->get_device()));

    m_total_indices = mesh.indices.size();
}

void joj::SceneObject3D::bind(IRenderer* renderer)
{
    u32 stride = sizeof(Vertex::PosColorNormal);
    u32 offset = 0;

    m_vb.bind(renderer->get_cmd_list(), 0, 1, &stride, &offset);
    m_ib.bind(renderer->get_cmd_list(), joj::DataFormat::R32_UINT, offset);
}

void joj::SceneObject3D::set_position(const JVector3 v)
{
    m_pos = v;
}

void joj::SceneObject3D::translate(const f32 dx, const f32 dy, const f32 dz)
{
    m_pos.x += dx;
    m_pos.y += dy;
    m_pos.z += dz;
}

joj::JVector3 joj::SceneObject3D::get_position() const
{
    return JVector3(m_pos);
}

u32 joj::SceneObject3D::get_total_indices() const
{
    return m_total_indices;
}