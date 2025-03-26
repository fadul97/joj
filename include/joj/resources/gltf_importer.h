#ifndef _JOJ_GLTF_IMPORTER_H
#define _JOJ_GLTF_IMPORTER_H

#include "joj/core/defines.h"

#include "joj/core/error_code.h"
#include <vector>
#include "joj/core/math/vector3.h"
#include "joj/utils/json_value.h"

namespace joj
{
    struct JOJ_API GLTFVertex
    {
        Vector3 pos;
        Vector3 normal;
    };

    class JOJ_API GLTFImporter
    {
    public:
        GLTFImporter();
        GLTFImporter(const char* filename);
        ~GLTFImporter();

        ErrorCode load();
        
        std::vector<GLTFVertex> get_vertices();
        
        void print_scene_info();
        void print_node_info();
        void print_mesh_info();
        void print_vertex_data();

        std::vector<joj::Vector3> m_positions;
        std::vector<joj::Vector3> m_normals;
        std::vector<u16> m_indices;

    private:
        const char* m_gltf_filename;
        std::string m_bin_filename;
        
        std::vector<u8> m_binary_data;

    public:
        i32 m_positions_byte_offset;
        i32 m_normals_byte_offset;
        i32 m_indices_byte_offset;

        i32 m_positions_count;
        i32 m_normals_count;
        i32 m_indices_count;

        JsonValue m_root;
        
        b8 parse_json();
        b8 load_binary_file();

        void extract_vertices();
        void extract_accessors();
        void extract_buffer_views();

        void load_positions_from_buffer(const size_t byte_offset, const size_t count);
        void load_normals_from_buffer(const size_t byte_offset, const size_t count);
        void load_indices_from_buffer(const size_t byte_offset, const size_t count);
    };
}

#endif // _JOJ_GLTF_IMPORTER_H