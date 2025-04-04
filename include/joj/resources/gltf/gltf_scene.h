#ifndef _JOJ_GLTF_SCENE_H
#define _JOJ_GLTF_SCENE_H

#include "joj/core/defines.h"

#include <string>
#include <vector>
#include "joj/renderer/vertex.h"
#include "joj/resources/submesh.h"

namespace joj
{
    class IRenderer;

    class JOJ_API GLTFScene
    {
    public:
        GLTFScene();
        ~GLTFScene();

        void add_vertices(const std::vector<Vertex::ColorTanPosNormalTex>& vertices);
        void add_indices(const std::vector<u16>& indices);
        void add_submesh(const Submesh& submesh);

        void write_vertices_to_file(const char* filename) const;
        void write_indices_to_file(const char* filename) const;
        void write_vertices_and_indices_to_file(const char* filename) const;
        void write_submeshes_to_file(const char* filename) const;
        void write_submesh_data_to_file(const char* filename) const;

        void set_name(const std::string& name);
        void set_vertices(const std::vector<Vertex::ColorTanPosNormalTex>& vertices);
        void set_indices(const std::vector<u16>& indices);
        
        const std::string& get_name() const;
        const std::vector<Vertex::ColorTanPosNormalTex>& get_vertex_data() const;
        const std::vector<u16>& get_index_data() const;
        const std::vector<Submesh>& get_submeshes() const;

        const i32 get_vertex_count() const;
        const i32 get_index_count() const;
        const i32 get_submesh_count() const;

        void print_info() const;

        void draw(IRenderer* renderer) const;
        void draw_mesh_index(IRenderer* renderer, const u32 submesh_index) const;

    private:
        std::string m_name;
        std::vector<Vertex::ColorTanPosNormalTex> m_vertices;
        std::vector<u16> m_indices;
        std::vector<Submesh> m_submeshes;
    };
}

#endif // _JOJ_GLTF_SCENE_H