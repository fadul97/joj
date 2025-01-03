#ifndef _JOJ_MESH_GEOMETRY_OLD_H
#define _JOJ_MESH_GEOMETRY_OLD_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "math/jmath.h"
#include <vector>
#include "renderer/renderer.h"
#include "renderer/shader.h"

namespace joj
{
    struct VertexOLD
    {
        JFloat3 position;
        JFloat2 texture;
        JFloat3 normal;
    };

    struct ConstantBuffer
    {
        JFloat4x4 wvp;
        JFloat4x4 worldMatrix;
        JFloat4x4 viewMatrix;
        JFloat4x4 projectionMatrix;
    };

    enum class MeshGeometryType
    {
        None,
        OBJ,
    };

    class JAPI MeshGeometryOld
    {
    public:
        MeshGeometryOld();
        MeshGeometryOld(const std::string& filename, MeshGeometryType type);
        virtual ~MeshGeometryOld();

        void translate(f32 dx, f32 dy, f32 dz);
        void move_to(f32 px, f32 py, f32 pz);

        JFloat3 get_position() const;

        const VertexOLD* get_vertex_data() const;
        const u32* get_index_data() const;

        u32 get_vertex_count() const;
        u32 get_index_count() const;

        virtual Shader& get_shader() = 0;

        virtual void setup(GraphicsDevice& device) = 0;

        virtual void update(const JFloat4x4 view, const JFloat4x4 proj, const f32 dt) = 0;

        virtual void draw(GraphicsDevice& device, CommandList& cmd_list) = 0;

    protected:
        MeshGeometryType m_type;
        JFloat3 m_position;
        std::vector<VertexOLD> m_vertices;
        std::vector<u32> m_indices;
        u32 m_vertex_count;
        u32 m_index_count;
    };

    inline void MeshGeometryOld::translate(const f32 dx, const f32 dy, const f32 dz)
    { m_position.x += dx; m_position.y += dy; m_position.z += dz; }

    inline void MeshGeometryOld::move_to(const f32 px, const f32 py, const f32 pz)
    { m_position.x = px; m_position.y = py; m_position.z = pz; }

    inline JFloat3 MeshGeometryOld::get_position() const
    { return m_position; }

    inline const VertexOLD* MeshGeometryOld::get_vertex_data() const
    { return m_vertices.data(); }

    inline const u32* MeshGeometryOld::get_index_data() const
    { return m_indices.data(); }

    inline u32 MeshGeometryOld::get_vertex_count() const
    { return m_vertex_count; }

    inline u32 MeshGeometryOld::get_index_count() const
    { return m_index_count; }
}

#endif // _JOJ_MESH_GEOMETRY_OLD_H