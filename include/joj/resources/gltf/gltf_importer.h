#ifndef _JOJ_GLTF_IMPORTER_H
#define _JOJ_GLTF_IMPORTER_H

#include "joj/core/defines.h"

#include "joj/core/error_code.h"
#include <string>
#include <vector>
#include "joj/core/math/vector3.h"
#include "gltf_sampler.h"
#include "gltf_buffer_view.h"
#include "gltf_accessor.h"
#include "joj/resources/buffer.h"
#include "joj/resources/animation.h"
#include "joj/utils/json_value.h"

namespace joj
{
    class JOJ_API GLTFImporter
    {
    public:
        GLTFImporter();
        ~GLTFImporter();

        ErrorCode load(const char* file_path);

    private:
        std::string m_gltf_filename;
        std::string m_bin_filename;

        std::vector<Vector3> m_positions;
        std::vector<Vector3> m_normals;
        std::vector<u16> m_indices;
        std::vector<Animation> m_animations;
        std::vector<Vector3> m_translations;
        std::vector<Vector4> m_rotations;
        std::vector<Vector3> m_scales;
        std::vector<GLTFSampler> m_samplers;
        std::vector<GLTFAccessor> m_accessors;
        std::vector<GLTFBufferView> m_buffer_views;
        std::vector<Buffer> m_buffers;

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

        b8 load_buffers();
        void print_buffers();
    };
}

#endif // _JOJ_GLTF_IMPORTER_H