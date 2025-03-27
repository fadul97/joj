#include "joj/resources/gltf/gltf_importer.h"

#include <fstream>
#include <sstream>
#include "joj/utils/json_parser.h"
#include "joj/core/logger.h"
#include <algorithm>

joj::GLTFImporter::GLTFImporter()
    : m_gltf_filename(""), m_bin_filename("")
    , m_positions(), m_normals(), m_indices()
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
        return ErrorCode::FAILED;

    if (!load_buffers())
        return ErrorCode::FAILED;
    print_buffers();

    if (!load_buffer_views())
        return ErrorCode::FAILED;
    print_buffer_views();

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

joj::Buffer joj::GLTFImporter::load_binary_file(const char* filename)
{
    // Open binary file
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        JOJ_ERROR(ErrorCode::FAILED, "Failed to open binary buffer file: %s", filename);
        return Buffer();
    }

    // Get file size
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Read binary data
    std::vector<u8> binary_data;
    binary_data.resize(size);
    file.read(reinterpret_cast<char*>(binary_data.data()), size);

    return Buffer(filename, BufferType::BYTE, binary_data);
}

b8 joj::GLTFImporter::load_buffers()
{
    if (!m_root.has_key("buffers"))
        return false;

    // Get buffer array
    auto buffers = m_root["buffers"].as_array();
    for (const auto& buffer : buffers)
    {
        // Get buffer URI and concatenate with the models folder
        if (buffer.has_key("uri"))
        {
            std::string uri = "models/";
            uri += buffer["uri"].as_string().c_str();

            JOJ_DEBUG("Loading buffer: %s", uri.c_str());

            // Buffer data for each buffer in the array
            Buffer data_buffer = load_binary_file(uri.c_str());
            if (data_buffer.type == BufferType::UNKNOWN)
                return false;

            // Add buffer to the vector
            m_buffers.push_back(std::move(data_buffer));
        }
        else
        {
            // URI is embedded in the JSON file
            // TODO: Implement this
            JOJ_ERROR(ErrorCode::FAILED, "Buffer URI is embedded in the JSON file.");
            return false;
        }
    }

    return true;
}

void joj::GLTFImporter::print_buffers()
{
    std::cout << "Total loaded buffers: " << m_buffers.size() << std::endl;

    i32 i = 0;
    for (const auto& buffer : m_buffers)
    {
        std::cout << "Buffer " << i << " (" << buffer.size << " bytes): " << std::endl;
        std::cout << "    Buffer filename: " << buffer.filename << std::endl;
        std::cout << "    Buffer type: " << buffer_type_to_string(buffer.type) << std::endl;
        ++i;
    }
}

b8 joj::GLTFImporter::load_buffer_views()
{
    if (!m_root.has_key("bufferViews"))
        return false;

    // Get buffer views array
    auto buffer_views = m_root["bufferViews"].as_array();
    i32 i = 0;
    for (const auto& buffer_view : buffer_views)
    {
        GLTFBufferView view;

        if (buffer_view.has_key("buffer"))
        {
            if (buffer_view["buffer"].is_int())
            {
                view.buffer = buffer_view["buffer"].as_int();
            }
            else
            {
                JOJ_ERROR(ErrorCode::FAILED, "BufferView[%d] buffer is not an integer.", i);
                return false;
            }
        }
        else
        {
            JOJ_ERROR(ErrorCode::FAILED, "BufferView[%d] does not have key 'buffer' is not an integer.", i);
            return false;
        }

        if (buffer_view.has_key("byteLength"))
        {
            if (buffer_view["byteLength"].is_int())
            {
                view.byte_length = buffer_view["byteLength"].as_int();
            }
            else
            {
                JOJ_ERROR(ErrorCode::FAILED, "BufferView[%d] byte length is not an integer.", i);
                return false;
            }
        }
        else
        {
            JOJ_ERROR(ErrorCode::FAILED, "BufferView[%d] does not have key 'byteLength'.", i);
            return false;
        }


        if (buffer_view.has_key("byteOffset"))
        {
            if (buffer_view["byteOffset"].is_int())
            {
                view.byte_offset = buffer_view["byteOffset"].as_int();
            }
            else
            {
                JOJ_ERROR(ErrorCode::FAILED, "BufferView[%d] byte offset is not an integer.", i);
                return false;
            }
        }
        else
        {
            JOJ_ERROR(ErrorCode::FAILED, "BufferView[%d] does not have key 'byteOffset'.", i);
            return false;
        }

        if (buffer_view.has_key("byteStride"))
        {
            if (buffer_view["byteStride"].is_int())
            {
                view.byte_stride = buffer_view["byteStride"].as_int();
            }
            else
            {
                JOJ_ERROR(ErrorCode::FAILED, "BufferView[%d] byte stride is not an integer.", i);
                return false;
            }
        }
        else
        {
            JOJ_WARN("BufferView[%d] does not have key 'byteStride' Assuming 0.", i);
            view.byte_stride = 0;
        }

        if (buffer_view.has_key("target"))
        {
            if (buffer_view["target"].is_int())
            {
                const i32 target = buffer_view["target"].as_int();
                if (target == BUFFER_VIEW_TARGET_ARRAY_BUFFER)
                    view.target = BufferViewTarget::ARRAY_BUFFER;
                else if (target == BUFFER_VIEW_TARGET_ELEMENT_ARRAY_BUFFER)
                    view.target = BufferViewTarget::ELEMENT_ARRAY_BUFFER;
                else
                    view.target = BufferViewTarget::UNKNOWN;
            }
            else
            {
                JOJ_ERROR(ErrorCode::FAILED, "BufferView[%d] target is not an integer.", i);
                return false;
            }
        }
        else
        {
            JOJ_ERROR(ErrorCode::FAILED, "BufferView[%d] does not have key 'target'.", i);
            return false;
        }

        m_buffer_views.push_back(view);
    }
}

void joj::GLTFImporter::print_buffer_views()
{
    std::cout << "Total loaded buffer views: " << m_buffer_views.size() << std::endl;

    i32 i = 0;
    for (const auto& view : m_buffer_views)
    {
        std::cout << "BufferView " << i << ": " << std::endl;
        std::cout << "    Buffer: " << view.buffer << std::endl;
        std::cout << "    Byte offset: " << view.byte_offset << std::endl;
        std::cout << "    Byte length: " << view.byte_length << std::endl;
        std::cout << "    Byte stride: " << view.byte_stride << std::endl;
        std::cout << "    Target: " << buffer_view_target_to_string(view.target) << std::endl;
        ++i;
    }
}