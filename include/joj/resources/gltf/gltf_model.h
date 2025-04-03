#ifndef _JOJ_GLTF_MODEL_H
#define _JOJ_GLTF_MODEL_H

#include "joj/core/defines.h"

#include <vector>
#include "gltf_mesh.h"
#include "gltf_node.h"
#include "joj/renderer/vertex.h"
#include "joj/resources/submesh.h"

namespace joj
{
    struct Mesh;

    class JOJ_API GLTFModel
    {
    public:
        GLTFModel();
        ~GLTFModel();

        void clear_root_nodes();

        void add_gltf_mesh(const GLTFMesh& mesh);
        void add_gltf_node(const GLTFNode& node);
        void add_root_node(const i32 node_index);
        void add_root_node(const std::string& node_name);
        void add_aggregated_mesh(const Mesh& mesh);

        void set_gltf_meshes(const std::vector<GLTFMesh>& meshes);
        void set_gltf_nodes(const std::vector<GLTFNode>& nodes);
        void set_root_nodes(const std::vector<i32>& root_nodes);
        void set_aggregated_meshes(const std::vector<Mesh>& meshes);

        const std::vector<Vertex::ColorTanPosNormalTex>& get_vertices() const;
        const std::vector<u16>& get_indices() const;
        const std::vector<Submesh>& get_submeshes() const;

        const i32 get_vertex_count() const;
        const i32 get_index_count() const;
        const i32 get_submesh_count() const;

        const std::vector<GLTFMesh>& get_gltf_meshes() const;
        const GLTFMesh* get_gltf_mesh(const i32 index) const;
        const GLTFMesh* get_gltf_mesh(const std::string& name) const;
        
        const std::vector<GLTFNode>& get_nodes() const;
        const GLTFNode* get_node(const i32 index) const;
        const GLTFNode* get_node(const std::string& name) const;
        const GLTFNode* get_node_by_mesh(const i32 mesh_index) const;
        const GLTFNode* get_node_by_mesh(const std::string& mesh_name) const;

        const std::vector<i32>& get_root_nodes() const;
        const i32 get_root_node(const i32 index) const;
        const i32 get_root_node(const std::string& name) const;
        const i32 get_root_node_by_mesh(const i32 mesh_index) const;
        const i32 get_root_node_by_mesh(const std::string& mesh_name) const;

        const i32 get_gltf_meshes_count() const;
        const i32 get_gltf_nodes_count() const;
        const i32 get_root_nodes_count() const;
        const i32 get_aggregated_meshes_count() const;

        void print_info() const;
        void print_hierarchy() const;
        void print_meshes() const;
        void print_nodes() const;

        void write_data_to_file(const char* filename) const;

    private:
        std::vector<Vertex::ColorTanPosNormalTex> m_vertices;
        std::vector<u16> m_indices;
        std::vector<Submesh> m_submeshes;
        std::vector<GLTFMesh> m_gltf_meshes;
        std::vector<GLTFNode> m_gltf_nodes;
        std::vector<i32> m_root_nodes;
        std::vector<Mesh> m_aggregated_meshes;
    };
}

#endif // _JOJ_GLTF_MODEL_H