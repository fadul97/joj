#ifndef _JOJ_GLTF_IMPORTER_H
#define _JOJ_GLTF_IMPORTER_H

#include "joj/core/defines.h"

#include "joj/core/error_code.h"
#include <string>
#include <vector>
#include "joj/core/math/vector3.h"
#include "gltf_animation_sampler.h"
#include "gltf_buffer_view.h"
#include "gltf_accessor.h"
#include "gltf_mesh.h"
#include "gltf_animation.h"
#include "gltf_skin.h"
#include "gltf_scene.h"
#include "gltf_node.h"
#include "joj/resources/buffer.h"
#include "joj/resources/animation.h"
#include "joj/utils/json_value.h"
#include "joj/resources/mesh.h"

namespace joj
{
    struct GLTFVertex
    {
        Vector3 pos;
        Vector4 color;
        Vector3 normal;
    };

    constexpr i32 BUFFER_VIEW_TARGET_ARRAY_BUFFER = 34962;
    constexpr i32 BUFFER_VIEW_TARGET_ELEMENT_ARRAY_BUFFER = 34963;

    class JOJ_API GLTFImporter
    {
    public:
        GLTFImporter();
        ~GLTFImporter();

        ErrorCode load(const char* file_path);

        void get_vertices(std::vector<GLTFVertex>& vertices);
        void get_indices(std::vector<u16>& indices);
        void get_vertices_and_indices(std::vector<GLTFVertex>& vertices, std::vector<u16>& indices);

        void get_meshes(std::vector<GLTFMesh>& meshes);

        std::vector<GLTFAccessor>& get_accessors();
        const GLTFAccessor& get_accessor(const u32 index) const;

        const Buffer& get_buffer(const GLTFAccessor& accessor) const;
        const GLTFBufferView& get_buffer_view(const GLTFAccessor& accessor) const;

        void setup_mesh(GLTFMesh& gltf_mesh, Mesh& mesh);

        template <typename T>
        std::vector<T> read_buffer(const u32 accessor_index) const
        {
            const auto& accessor = get_accessor(accessor_index);
            const auto& bufferView = get_buffer_view(accessor);
            const auto& buffer = get_buffer(accessor);

            return read_buffer_internal<T>(buffer, accessor, bufferView);
        }

    private:
        std::string m_gltf_filename;
        std::string m_bin_filename;

        std::vector<Vector3> m_positions;
        std::vector<Vector3> m_normals;
        std::vector<u16> m_indices;
        std::vector<Vector3> m_translations;
        std::vector<Vector4> m_rotations;
        std::vector<Vector3> m_scales;
        std::vector<GLTFAnimationSampler> m_samplers;
        std::vector<GLTFNode> m_nodes;
        std::vector<GLTFAccessor> m_accessors;
        std::vector<GLTFBufferView> m_buffer_views;
        std::vector<Buffer> m_buffers;
        std::vector<GLTFMesh> m_meshes;
        std::vector<GLTFAnimation> m_animations;
        std::vector<GLTFSkin> m_skins;
        std::vector<GLTFScene> m_scenes;

        i32 m_positions_byte_offset;
        i32 m_normals_byte_offset;
        i32 m_indices_byte_offset;
        i32 m_animations_byte_offset;
        i32 m_translation_byte_offset;
        i32 m_rotation_byte_offset;
        i32 m_scale_byte_offset;

        i32 m_positions_count;
        i32 m_normals_count;
        i32 m_indices_count;
        i32 m_animations_count;
        i32 m_translation_count;
        i32 m_rotation_count;
        i32 m_scale_count;

        JsonValue m_root;

        b8 parse_json();

        Buffer load_binary_file(const char* filename);
        void load_data_buffer(const size_t byte_offset, const size_t count);

        b8 load_buffers();
        void print_buffers();

        b8 load_buffer_views();
        void print_buffer_views();

        b8 load_accessors();
        void print_accessors();

        b8 load_nodes();
        void print_nodes();

        b8 load_meshes();
        void print_meshes();

        b8 load_animations();
        void print_animations();

        b8 load_skins();
        void print_skins();

        b8 load_scenes();
        void print_scenes();

        template <typename T>
        std::vector<T> read_buffer_internal(const Buffer& buffer, const GLTFAccessor& accessor, const GLTFBufferView& bufferView) const
        {
            std::vector<T> data;
            size_t element_size = sizeof(T);
            size_t count = accessor.count;
            size_t stride = (bufferView.byte_stride > 0) ? bufferView.byte_stride : element_size;
            size_t start_offset = bufferView.byte_offset + accessor.byte_offset;

            data.resize(count);
            for (size_t i = 0; i < count; i++)
            {
                size_t offset = start_offset + (i * stride);
                memcpy(&data[i], &buffer.data[offset], element_size);
            }

            return data;
        }
    };
}

#endif // _JOJ_GLTF_IMPORTER_H