#ifndef _JOJ_GLTF_IMPORTER_H
#define _JOJ_GLTF_IMPORTER_H

#include "joj/core/defines.h"

#include "joj/core/error_code.h"
#include <vector>
#include "joj/core/math/vector3.h"
#include "joj/core/math/vector4.h"
#include <string>
#include "joj/utils/json_value.h"

namespace joj
{
    struct JOJ_API GLTFKeyFrame
    {
        f32 time;
        Vector3 translation;
        Vector3 scale;
        Vector3 rotation;
    };

    struct JOJ_API GLTFAnimationChannel
    {
        std::string path;
        i32 target_node_index;
        std::vector<GLTFKeyFrame> keyframes;
    };

    struct JOJ_API GLTFAnimation
    {
        std::string name;
        std::vector<GLTFAnimationChannel> channels;
    };

    // Representa um channel da animação
    struct JOJ_API GLTFChannel
    {
        i32 sampler; // Índice do sampler (para ler o input e output)
        std::string path; // Caminho do target (ex.: "translation", "rotation", "scale")
        i32 node; // Índice do node alvo da animação
    };

    // Representa um sampler de animação
    struct JOJ_API GLTFSampler
    {
        i32 input; // Índice do acessador de entrada (tempo da animação)
        i32 output; // Índice do acessador de saída (valores da animação)
        std::string interpolation; // Tipo de interpolação ("LINEAR", "STEP", etc.)
    };

    struct JOJ_API GLTFBufferView
    {
        i32 buffer;      // Índice do buffer ao qual este buffer view pertence
        i32 byteOffset;  // Deslocamento em bytes dentro do buffer
        i32 byteLength;  // Comprimento dos dados dentro do buffer view
        i32 byteStride;  // O espaçamento entre elementos, caso seja necessário (por exemplo, 3*float para um vetor de 3 floats)
    };

    struct JOJ_API GLTFBuffer
    {
        std::vector<u8> data; // Dados binários do buffer
    };

    struct JOJ_API GLTFAccessor
    {
        std::string type; // Tipo de dado, por exemplo, "VEC3", "SCALAR", etc.
        i32 componentType; // Tipo de componente, por exemplo, 5126 para float
        i32 count; // Número de elementos
        i32 bufferView; // Índice do buffer view
        i32 byteOffset; // Offset em bytes dentro do buffer
    };

    struct JOJ_API GLTFNode
    {
        std::string name;  // Nome do objeto (ex: "Cube")
        Vector3 position = {0, 0, 0};  // Posição do objeto
        Vector4 rotation = {0, 0, 0, 1};  // Rotação como quaternion
        Vector3 scale = {1, 1, 1};  // Escala do objeto
    
        // Métodos auxiliares para modificar transformações, se necessário
        void SetPosition(const Vector3& pos) { position = pos; }
        void SetRotation(const Vector4& rot) { rotation = rot; }
        void SetScale(const Vector3& scl) { scale = scl; }
    };

    void apply_animation(GLTFAnimation& animation, f32 time, GLTFNode& node);
    void apply_all_animations(GLTFAnimation& animation, f32 time, GLTFNode& node);

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

        std::vector<f32> load_buffer_data(const GLTFBufferView& buffer_view);
        
        void load_buffers();
        void load_buffer_views();
        void load_accessors();
        void load_samplers();
        void load_animations();
        std::vector<u8> load_binary_data(const std::string& uri);

        std::vector<GLTFVertex> get_vertices();
        std::vector<GLTFAnimation>& get_animations();
        
        void print_root();
        void print_scene_info();
        void print_node_info();
        void print_mesh_info();
        void print_vertex_data();
        void print_animation_data();
        void print_animation_info();
        void print_translation_data();
        void print_rotation_data();
        void print_scale_data();

        std::vector<joj::Vector3> m_positions;
        std::vector<joj::Vector3> m_normals;
        std::vector<u16> m_indices;
        std::vector<GLTFAnimation> m_animations;
        std::vector<Vector3> m_translations;
        std::vector<Vector4> m_rotations;
        std::vector<Vector3> m_scales;
        std::vector<GLTFSampler> m_samplers;
        std::vector<GLTFAccessor> m_accessors;
        std::vector<GLTFBufferView> m_bufferViews;
        std::vector<GLTFBuffer> m_buffers;

    private:
        const char* m_gltf_filename;
        std::string m_bin_filename;
        
        std::vector<u8> m_binary_data;

    public:
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
        b8 load_binary_file();

        void extract_vertices();
        void extract_accessors();
        void extract_buffer_views();

        void load_positions_from_buffer(const size_t byte_offset, const size_t count);
        void load_normals_from_buffer(const size_t byte_offset, const size_t count);
        void load_indices_from_buffer(const size_t byte_offset, const size_t count);
        void load_animations_from_buffer(const size_t byte_offset, const size_t count);
        void load_translation_from_buffer(const size_t byte_offset, const size_t count);
        void load_rotation_from_buffer(const size_t byte_offset, const size_t count);
        void load_scale_from_buffer(const size_t byte_offset, const size_t count);

        void setup_animations();
    };
}

#endif // _JOJ_GLTF_IMPORTER_H