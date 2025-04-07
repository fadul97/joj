#ifndef _JOJ_MESH_H
#define _JOJ_MESH_H

#include "joj/core/defines.h"

#include <vector>
#include "joj/renderer/vertex.h"
#include "submesh.h"

namespace joj
{
    class IRenderer;

    class JOJ_API Mesh
    {
    public:
        Mesh();
        ~Mesh();

        void add_vertices(const std::vector<Vertex::ColorTanPosNormalTex>& vertices);
        void add_indices(const std::vector<u32>& indices);
        void add_submesh(const Submesh& submesh);

        void set_name(const char* name);
        void set_vertices(const std::vector<Vertex::ColorTanPosNormalTex>& vertices);
        void set_indices(const std::vector<u32>& indices);
        void set_submeshes(const std::vector<Submesh>& submeshes);
        
        const std::string& get_name() const;
        const std::vector<Vertex::ColorTanPosNormalTex>& get_vertex_data() const;
        const std::vector<u32>& get_index_data() const;
        const std::vector<Submesh>& get_submeshes() const;

        const i32 get_vertex_count() const;
        const i32 get_index_count() const;
        const i32 get_submesh_count() const;

        void draw(IRenderer* renderer) const;
        void draw_mesh_index(IRenderer* renderer, const u32 submesh_index) const;

    private:
        std::string m_name;
        std::vector<Vertex::ColorTanPosNormalTex> m_vertices;
        std::vector<u32> m_indices;
        std::vector<Submesh> m_submeshes;
    };
}

#endif // _JOJ_MESH_H