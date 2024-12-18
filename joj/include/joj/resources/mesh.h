#ifndef _JOJ_MESH_H
#define _JOJ_MESH_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "math/jmath.h"
#include <vector>
#include "renderer/renderer.h"
#include "renderer/shader.h"

namespace joj
{
    struct Vertex
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

    enum class MeshType
    {
        None,
        OBJ,
    };

    class JAPI Mesh
    {
    public:
        Mesh();
        Mesh(const std::string& filename, MeshType type);
        virtual ~Mesh();

        void translate(f32 dx, f32 dy, f32 dz);
        void move_to(f32 px, f32 py, f32 pz);

        JFloat3 get_position() const;

        const Vertex* get_vertex_data() const;
        const u32* get_index_data() const;

        u32 get_vertex_count() const;
        u32 get_index_count() const;

        virtual Shader& get_shader() = 0;

        virtual void setup(GraphicsDevice& device) = 0;

        virtual void update(const JFloat4x4 view, const JFloat4x4 proj, const f32 dt) = 0;

        virtual void draw(GraphicsDevice& device, CommandList& cmd_list) = 0;

    protected:
        MeshType m_type;
        JFloat3 m_position;
        std::vector<Vertex> m_vertices;
        std::vector<u32> m_indices;
        u32 m_vertex_count;
        u32 m_index_count;
    };

    inline void Mesh::translate(const f32 dx, const f32 dy, const f32 dz)
    { m_position.x += dx; m_position.y += dy; m_position.z += dz; }

    inline void Mesh::move_to(const f32 px, const f32 py, const f32 pz)
    { m_position.x = px; m_position.y = py; m_position.z = pz; }

    inline JFloat3 Mesh::get_position() const
    { return m_position; }

    inline const Vertex* Mesh::get_vertex_data() const
    { return m_vertices.data(); }

    inline const u32* Mesh::get_index_data() const
    { return m_indices.data(); }

    inline u32 Mesh::get_vertex_count() const
    { return m_vertex_count; }

    inline u32 Mesh::get_index_count() const
    { return m_index_count; }
}

#endif // _JOJ_MESH_H