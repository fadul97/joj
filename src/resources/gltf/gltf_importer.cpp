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
    , m_root(), m_nodes()
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

    if (!load_accessors())
        return ErrorCode::FAILED;
    print_accessors();

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
            JOJ_WARN("BufferView[%d] does not have key 'byteOffset' Assuming 0.", i);
            view.byte_offset = 0;
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
            JOJ_ERROR(ErrorCode::FAILED, "BufferView[%d] does not have key 'target'. Assuming ANY", i);
            view.target = BufferViewTarget::ANY;
        }

        m_buffer_views.push_back(view);
        ++i;
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

b8 joj::GLTFImporter::load_accessors()
{
    if (!m_root.has_key("accessors"))
        return false;

    // Get accessors array
    auto accessors = m_root["accessors"].as_array();
    i32 i = 0;
    for (const auto& accessor : accessors)
    {
        GLTFAccessor acc;

        if (accessor.has_key("type"))
        {
            if (accessor["type"].is_string())
            {
                const std::string type = accessor["type"].as_string();
                if (type == "SCALAR")
                    acc.data_type = DataType::SCALAR;
                else if (type == "VEC2")
                    acc.data_type = DataType::VEC2;
                else if (type == "VEC3")
                    acc.data_type = DataType::VEC3;
                else if (type == "VEC4")
                    acc.data_type = DataType::VEC4;
                else if (type == "MAT2")
                    acc.data_type = DataType::MAT2;
                else if (type == "MAT3")
                    acc.data_type = DataType::MAT3;
                else if (type == "MAT4")
                    acc.data_type = DataType::MAT4;
                else
                    acc.data_type = DataType::UNKNOWN;
            }
            else
            {
                JOJ_ERROR(ErrorCode::FAILED, "Accessor[%d] type is not a string.", i);
                return false;
            }
        }
        else
        {
            JOJ_ERROR(ErrorCode::FAILED, "Accessor[%d] does not have key 'type'.", i);
            return false;
        }

        if (accessor.has_key("componentType"))
        {
            if (accessor["componentType"].is_int())
            {
                const i32 component_type = accessor["componentType"].as_int();
                if (component_type == 5120)
                    acc.component_type = ComponentType::BYTE;
                else if (component_type == 5121)
                    acc.component_type = ComponentType::UNSIGNED_BYTE;
                else if (component_type == 5122)
                    acc.component_type = ComponentType::I16;
                else if (component_type == 5123)
                    acc.component_type = ComponentType::U16;
                else if (component_type == 5125)
                    acc.component_type = ComponentType::U32;
                else if (component_type == 5126)
                    acc.component_type = ComponentType::F32;
                else
                    acc.component_type = ComponentType::UNKNOWN;
            }
            else
            {
                JOJ_ERROR(ErrorCode::FAILED, "Accessor[%d] component type is not an integer.", i);
                return false;
            }
        }
        else
        {
            JOJ_ERROR(ErrorCode::FAILED, "Accessor[%d] does not have key 'componentType'.", i);
            return false;
        }

        if (accessor.has_key("count"))
        {
            if (accessor["count"].is_int())
            {
                acc.count = accessor["count"].as_int();
            }
            else
            {
                JOJ_ERROR(ErrorCode::FAILED, "Accessor[%d] count is not an integer.", i);
                return false;
            }
        }
        else
        {
            JOJ_ERROR(ErrorCode::FAILED, "Accessor[%d] does not have key 'count'.", i);
            return false;
        }

        if (accessor.has_key("bufferView"))
        {
            if (accessor["bufferView"].is_int())
            {
                acc.buffer_view = accessor["bufferView"].as_int();
            }
            else
            {
                JOJ_ERROR(ErrorCode::FAILED, "Accessor[%d] buffer view is not an integer.", i);
                return false;
            }
        }
        else
        {
            JOJ_ERROR(ErrorCode::FAILED, "Accessor[%d] does not have key 'bufferView'.", i);
            return false;
        }

        if (accessor.has_key("byteOffset"))
        {
            if (accessor["byteOffset"].is_int())
            {
                acc.byte_offset = accessor["byteOffset"].as_int();
            }
            else
            {
                JOJ_ERROR(ErrorCode::FAILED, "Accessor[%d] byte offset is not an integer.", i);
                return false;
            }
        }
        else
        {
            JOJ_WARN("Accessor[%d] does not have key 'byteOffset'. Assuming 0.", i);
            acc.byte_offset = 0;
        }

        m_accessors.push_back(acc);
        ++i;
    }
}

void joj::GLTFImporter::print_accessors()
{
    std::cout << "Total loaded accessors: " << m_accessors.size() << std::endl;

    i32 i = 0;
    for (const auto& acc : m_accessors)
    {
        std::cout << "Accessor " << i << ": " << std::endl;
        std::cout << "    Data type: " << data_type_to_string(acc.data_type) << std::endl;
        std::cout << "    Component type: " << component_type_to_string(acc.component_type) << std::endl;
        std::cout << "    Count: " << acc.count << std::endl;
        std::cout << "    Buffer view: " << acc.buffer_view << std::endl;
        std::cout << "    Byte offset: " << acc.byte_offset << std::endl;
        ++i;
    }
}

b8 joj::GLTFImporter::load_nodes()
{
    if (!m_root.has_key("nodes"))
        return false;

    // Get nodes array
    auto nodes = m_root["nodes"].as_array();
    i32 i = 0;
    for (const auto& node : nodes)
    {
        SceneNode n;

        if (node.has_key("name"))
        {
            if (node["name"].is_string())
            {
                const std::string node_name = node["name"].as_string();
                n.set_name(node_name.c_str());
            }
            else
            {
                JOJ_ERROR(ErrorCode::FAILED, "Node[%d] name is not a string.", i);
                return false;
            }
        }
        else
        {
            JOJ_WARN("Node[%d] does not have key 'name'.", i);
            const std::string node_name = "Node" + std::to_string(i);
            n.set_name(node_name.c_str());
        }

        if (node.has_key("translation"))
        {
            if (node["translation"].is_array())
            {
                auto translation = node["translation"].as_array();
                if (translation.size() == 3)
                {
                    n.set_position(Vector3(translation[0].as_float(), translation[1].as_float(), translation[2].as_float()));
                }
                else
                {
                    JOJ_ERROR(ErrorCode::FAILED, "Node[%d] translation is not a 3-element array.", i);
                    return false;
                }
            }
            else
            {
                JOJ_ERROR(ErrorCode::FAILED, "Node[%d] translation is not an array.", i);
                return false;
            }
        }
        else
        {
            JOJ_WARN("Node[%d] does not have key 'translation'.", i);
        }

        if (node.has_key("rotation"))
        {
            if (node["rotation"].is_array())
            {
                auto rotation = node["rotation"].as_array();
                if (rotation.size() == 4)
                {
                    n.set_rotation(Vector4(rotation[0].as_float(), rotation[1].as_float(), rotation[2].as_float(), rotation[3].as_float()));
                }
                else
                {
                    JOJ_ERROR(ErrorCode::FAILED, "Node[%d] rotation is not a 4-element array.", i);
                    return false;
                }
            }
            else
            {
                JOJ_ERROR(ErrorCode::FAILED, "Node[%d] rotation is not an array.", i);
                return false;
            }
        }
        else
        {
            JOJ_WARN("Node[%d] does not have key 'rotation'.", i);
        }

        if (node.has_key("scale"))
        {
            if (node["scale"].is_array())
            {
                auto scale = node["scale"].as_array();
                if (scale.size() == 3)
                {
                    n.set_scale(Vector3(scale[0].as_float(), scale[1].as_float(), scale[2].as_float()));
                }
                else
                {
                    JOJ_ERROR(ErrorCode::FAILED, "Node[%d] scale is not a 3-element array.", i);
                    return false;
                }
            }
            else
            {
                JOJ_ERROR(ErrorCode::FAILED, "Node[%d] scale is not an array.", i);
                return false;
            }
        }
        else
        {
            JOJ_WARN("Node[%d] does not have key 'scale'.", i);
        }

        if (node.has_key("children"))
        {
            if (node["children"].is_array())
            {
                auto children = node["children"].as_array();
                for (const auto& child : children)
                {
                    if (child.is_int())
                    {
                        n.add_child(child.as_int());
                    }
                    else
                    {
                        JOJ_ERROR(ErrorCode::FAILED, "Node[%d] child is not an integer.", i);
                        return false;
                    }
                }
            }
            else
            {
                JOJ_ERROR(ErrorCode::FAILED, "Node[%d] children is not an array.", i);
                return false;
            }
        }
        else
        {
            JOJ_WARN("Node[%d] does not have key 'children'.", i);
        }

        if (node.has_key("mesh"))
        {
            if (node["mesh"].is_int())
            {
                const i32 mesh_index = node["mesh"].as_int();
                n.set_mesh(mesh_index);
            }
            else
            {
                JOJ_ERROR(ErrorCode::FAILED, "Node[%d] mesh is not an integer.", i);
                return false;
            }
        }
        else
        {
            JOJ_WARN("Node[%d] does not have key 'mesh'.", i);
        }

        if (node.has_key("skin"))
        {
            if (node["skin"].is_int())
            {
                const i32 skin_index = node["skin"].as_int();
                n.set_skin(skin_index);
            }
            else
            {
                JOJ_ERROR(ErrorCode::FAILED, "Node[%d] skin is not an integer.", i);
                return false;
            }
        }
        else
        {
            JOJ_WARN("Node[%d] does not have key 'skin'.", i);
        }

        // TODO: Read camera, weights, extensions, extras

        m_nodes.push_back(n);
        ++i;
    }
}

void joj::GLTFImporter::print_nodes()
{
    std::cout << "Total loaded nodes: " << m_nodes.size() << std::endl;

    i32 i = 0;
    for (const auto& node : m_nodes)
    {
        std::cout << "Node " << i << ": " << std::endl;
        std::cout << "    Name: " << node.get_name() << std::endl;
        std::cout << "    Position: " << node.get_position().to_string() << std::endl;
        std::cout << "    Rotation: " << node.get_rotation().to_string() << std::endl;
        std::cout << "    Scale: " << node.get_scale().to_string() << std::endl;
        std::cout << "    Mesh: " << node.get_mesh() << std::endl;
        std::cout << "    Skin: " << node.get_skin() << std::endl;
        ++i;
    }
}