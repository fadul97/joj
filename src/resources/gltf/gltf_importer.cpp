#include "joj/resources/gltf/gltf_importer.h"

#include <fstream>
#include <sstream>
#include "joj/utils/json_parser.h"

joj::GLTFImporter::GLTFImporter()
    : m_gltf_filename(""), m_bin_filename("")
    , m_binary_data(), m_positions(), m_normals(), m_indices()
    , m_animations(), m_translations(), m_rotations(), m_scales()
    , m_samplers(), m_accessors(), m_buffer_views(), m_buffers()
    , m_positions_byte_offset(-1), m_normals_byte_offset(-1), m_indices_byte_offset(-1)
    , m_animations_byte_offset(-1), m_translation_byte_offset(-1), m_rotation_byte_offset(-1)
    , m_scale_byte_offset(-1), m_positions_count(-1), m_normals_count(-1), m_indices_count(-1)
    , m_animations_count(-1), m_translation_count(-1), m_rotation_count(-1), m_scale_count(-1)
    , m_root()
{
}

joj::GLTFImporter::~GLTFImporter()
{
}

joj::ErrorCode joj::GLTFImporter::load(const char* file_path)
{
    m_gltf_filename = file_path;

    if (!parse_json())
    {
        return ErrorCode::FAILED;
    }

    return ErrorCode::OK;
}

b8 joj::GLTFImporter::parse_json()
{
    // Open GLTF file
    std::ifstream file(m_gltf_filename);
    if (!file.is_open())
        return false;

    // Read file content
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string json_content = buffer.str();

    // Parse JSON content
    joj::JsonParser parser(json_content);
    m_root = parser.parse();

    if (parser.get_error_count() > 0)
        return false;

    return true;
}
