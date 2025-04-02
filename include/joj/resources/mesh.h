#ifndef _JOJ_MESH_H
#define _JOJ_MESH_H

#include "joj/core/defines.h"

#include "submesh.h"
#include "joj/core/error_code.h"
#include "gltf/gltf_mesh.h"
#include <vector>
#include "joj/renderer/vertex.h"

namespace joj
{
    class JOJ_API Mesh
    {
    public:
        Mesh();
        ~Mesh();

        void set_vertices(const std::vector<Vertex::ColorTanPosNormalTex>& vertices);
        void set_indices(const std::vector<u16>& indices);
        void set_submeshes(const std::vector<Submesh>& submeshes);

        const std::vector<Vertex::ColorTanPosNormalTex>& get_vertex_data() const;
        const std::vector<u16>& get_index_data() const;
        const std::vector<Submesh>& get_submeshes() const;

        u32 get_vertex_count() const;
        u32 get_index_count() const;
        u32 get_submesh_count() const;

    private:
        std::vector<Vertex::ColorTanPosNormalTex> m_vertices;
        std::vector<u16> m_indices;
        std::vector<Submesh> m_submeshes;
    };
}

#endif // _JOJ_MESH_H