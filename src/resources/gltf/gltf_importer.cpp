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
    // print_buffers();

    if (!load_buffer_views())
        return ErrorCode::FAILED;
    // print_buffer_views();

    if (!load_accessors())
        return ErrorCode::FAILED;
    // print_accessors();

    if (!load_nodes())
        return ErrorCode::FAILED;
    // print_nodes();

    if (!load_meshes())
        return ErrorCode::FAILED;
    // print_meshes();

    if (!load_animations())
        return ErrorCode::FAILED;
    // print_animations();

    if (!load_skins())
        return ErrorCode::FAILED;
    // print_skins();

    if (!load_scenes())
        return ErrorCode::FAILED;
    // print_scenes();

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
        return true;

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
        return true;

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
                // JOJ_ERROR(ErrorCode::FAILED, "BufferView[%d] buffer is not an integer.", i);
                return false;
            }
        }
        else
        {
            // JOJ_ERROR(ErrorCode::FAILED, "BufferView[%d] does not have key 'buffer' is not an integer.", i);
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
                // JOJ_ERROR(ErrorCode::FAILED, "BufferView[%d] byte length is not an integer.", i);
                return false;
            }
        }
        else
        {
            // JOJ_ERROR(ErrorCode::FAILED, "BufferView[%d] does not have key 'byteLength'.", i);
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
                // JOJ_ERROR(ErrorCode::FAILED, "BufferView[%d] byte offset is not an integer.", i);
                return false;
            }
        }
        else
        {
            // JOJ_WARN("BufferView[%d] does not have key 'byteOffset' Assuming 0.", i);
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
                // JOJ_ERROR(ErrorCode::FAILED, "BufferView[%d] byte stride is not an integer.", i);
                return false;
            }
        }
        else
        {
            // JOJ_WARN("BufferView[%d] does not have key 'byteStride' Assuming 0.", i);
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
                // JOJ_ERROR(ErrorCode::FAILED, "BufferView[%d] target is not an integer.", i);
                return false;
            }
        }
        else
        {
            // JOJ_ERROR(ErrorCode::FAILED, "BufferView[%d] does not have key 'target'. Assuming ANY", i);
            view.target = BufferViewTarget::ANY;
        }

        m_buffer_views.push_back(view);
        ++i;
    }

    return true;
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
        return true;

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
                // JOJ_ERROR(ErrorCode::FAILED, "Accessor[%d] type is not a string.", i);
                return false;
            }
        }
        else
        {
            // JOJ_ERROR(ErrorCode::FAILED, "Accessor[%d] does not have key 'type'.", i);
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
                // JOJ_ERROR(ErrorCode::FAILED, "Accessor[%d] component type is not an integer.", i);
                return false;
            }
        }
        else
        {
            // JOJ_ERROR(ErrorCode::FAILED, "Accessor[%d] does not have key 'componentType'.", i);
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
                // JOJ_ERROR(ErrorCode::FAILED, "Accessor[%d] count is not an integer.", i);
                return false;
            }
        }
        else
        {
            // JOJ_ERROR(ErrorCode::FAILED, "Accessor[%d] does not have key 'count'.", i);
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
                // JOJ_ERROR(ErrorCode::FAILED, "Accessor[%d] buffer view is not an integer.", i);
                return false;
            }
        }
        else
        {
            // JOJ_ERROR(ErrorCode::FAILED, "Accessor[%d] does not have key 'bufferView'.", i);
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
                // JOJ_ERROR(ErrorCode::FAILED, "Accessor[%d] byte offset is not an integer.", i);
                return false;
            }
        }
        else
        {
            // JOJ_WARN("Accessor[%d] does not have key 'byteOffset'. Assuming 0.", i);
            acc.byte_offset = 0;
        }

        m_accessors.push_back(acc);
        ++i;
    }

    return true;
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
        return true;

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
                // JOJ_ERROR(ErrorCode::FAILED, "Node[%d] name is not a string.", i);
                return false;
            }
        }
        else
        {
            // JOJ_WARN("Node[%d] does not have key 'name'.", i);
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
                    // JOJ_ERROR(ErrorCode::FAILED, "Node[%d] translation is not a 3-element array.", i);
                    return false;
                }
            }
            else
            {
                // JOJ_ERROR(ErrorCode::FAILED, "Node[%d] translation is not an array.", i);
                return false;
            }
        }
        else
        {
            // JOJ_WARN("Node[%d] does not have key 'translation'.", i);
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
                    // JOJ_ERROR(ErrorCode::FAILED, "Node[%d] rotation is not a 4-element array.", i);
                    return false;
                }
            }
            else
            {
                // JOJ_ERROR(ErrorCode::FAILED, "Node[%d] rotation is not an array.", i);
                return false;
            }
        }
        else
        {
            // JOJ_WARN("Node[%d] does not have key 'rotation'.", i);
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
                    // JOJ_ERROR(ErrorCode::FAILED, "Node[%d] scale is not a 3-element array.", i);
                    return false;
                }
            }
            else
            {
                // JOJ_ERROR(ErrorCode::FAILED, "Node[%d] scale is not an array.", i);
                return false;
            }
        }
        else
        {
            // JOJ_WARN("Node[%d] does not have key 'scale'.", i);
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
                        // JOJ_ERROR(ErrorCode::FAILED, "Node[%d] child is not an integer.", i);
                        return false;
                    }
                }
            }
            else
            {
                // JOJ_ERROR(ErrorCode::FAILED, "Node[%d] children is not an array.", i);
                return false;
            }
        }
        else
        {
            // JOJ_WARN("Node[%d] does not have key 'children'.", i);
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
                // JOJ_ERROR(ErrorCode::FAILED, "Node[%d] mesh is not an integer.", i);
                return false;
            }
        }
        else
        {
            // JOJ_WARN("Node[%d] does not have key 'mesh'.", i);
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
                // JOJ_ERROR(ErrorCode::FAILED, "Node[%d] skin is not an integer.", i);
                return false;
            }
        }
        else
        {
            // JOJ_WARN("Node[%d] does not have key 'skin'.", i);
        }

        // TODO: Read camera, weights, extensions, extras

        m_nodes.push_back(n);
        ++i;
    }

    return true;
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
        if (!node.get_children_indices().empty())
        {
            std::cout << "    Children: ";
            for (const auto& child : node.get_children_indices())
            std::cout << child << " ";
            std::cout << std::endl;
        }
        ++i;
    }
}

b8 joj::GLTFImporter::load_meshes()
{
    if (!m_root.has_key("meshes"))
        return true;

    auto meshes = m_root["meshes"].as_array();
    i32 i = 0;
    for (const auto& mesh : meshes)
    {
        GLTFMesh m;
        if (mesh.has_key("name"))
        {
            if (mesh["name"].is_string())
            {
                const std::string mesh_name = mesh["name"].as_string();
                m.name = mesh_name;
            }
            else
            {
                // JOJ_ERROR(ErrorCode::FAILED, "Mesh[%d] name is not a string.", i);
            }
        }
        else
        {
            // JOJ_WARN("Mesh[%d] does not have key 'name'.", i);
        }

        auto primitives = mesh["primitives"].as_array();
        i32 j = 0;
        for (const auto& primitive : primitives)
        {
            GLFTPrimitive p;
            if (primitive.has_key("mode"))
            {
                if (primitive["mode"].is_int())
                {
                    const i32 pmode = primitive["mode"].as_int();
                    // std::cout << "Mesh[" << i << "] Primitive[" << j << "] mode: " << pmode << std::endl;
                    switch (pmode)
                    {
                    case 0:
                        p.mode = PrimitiveMode::POINTS;
                        break;
                    case 1:
                        p.mode = PrimitiveMode::LINES;
                        break;
                    case 2:
                        p.mode = PrimitiveMode::LINE_LOOP;
                        break;
                    case 3:
                        p.mode = PrimitiveMode::LINE_STRIP;
                        break;
                    case 4:
                        p.mode = PrimitiveMode::TRIANGLES;
                        break;
                    case 5:
                        p.mode = PrimitiveMode::TRIANGLE_STRIP;
                        break;
                    case 6:
                        p.mode = PrimitiveMode::TRIANGLE_FAN;
                        break;
                    default:
                        p.mode = PrimitiveMode::UNKNOWN;
                    }
                }
                else
                {
                    // JOJ_ERROR(ErrorCode::FAILED, "Mesh[%d] Primitive[%d] mode is not an integer.", i, j);
                }
            }
            else
            {
                // JOJ_WARN("Mesh[%d] Primitive[%d] does not have key 'mode'.", i, j);
            }

            if (primitive.has_key("material"))
            {
                if (primitive["material"].is_int())
                {
                    const i32 material_index = primitive["material"].as_int();
                    p.material_index = material_index;
                }
                else
                {
                    // JOJ_ERROR(ErrorCode::FAILED, "Mesh[%d] Primitive[%d] material index is not an integer.", i, j);
                }
            }
            else
            {
                // JOJ_WARN("Mesh[%d] Primitive[%d] does not have key 'material'.", i, j);
            }

            if (primitive.has_key("attributes"))
            {
                auto attributes = primitive["attributes"].as_object();
                if (attributes.find("POSITION") != attributes.end())
                {
                    if (attributes["POSITION"].is_int())
                    {
                        const i32 position_index = attributes["POSITION"].as_int();
                        p.position_acessor = position_index;
                    }
                    else
                    {
                        // JOJ_ERROR(ErrorCode::FAILED, "Mesh[%d] Primitive[%d] position index is not an integer.", i, j);
                    }
                }
                else
                {
                    // JOJ_WARN("Mesh[%d] Primitive[%d] does not have key 'POSITION'.", i, j);
                }

                if (attributes.find("NORMAL") != attributes.end())
                {
                    if (attributes["NORMAL"].is_int())
                    {
                        const i32 normal_index = attributes["NORMAL"].as_int();
                        p.normal_acessor = normal_index;
                    }
                    else
                    {
                        // JOJ_ERROR(ErrorCode::FAILED, "Mesh[%d] Primitive[%d] normal index is not an integer.", i, j);
                    }
                }
                else
                {
                    // JOJ_WARN("Mesh[%d] Primitive[%d] does not have key 'NORMAL'.", i, j);
                }

                if (attributes.find("TANGENT") != attributes.end())
                {
                    if (attributes["TANGENT"].is_int())
                    {
                        const i32 tangent_index = attributes["TANGENT"].as_int();
                        p.tangent_acessor = tangent_index;
                    }
                    else
                    {
                        // JOJ_ERROR(ErrorCode::FAILED, "Mesh[%d] Primitive[%d] tangent index is not an integer.", i, j);
                    }
                }
                else
                {
                    // JOJ_WARN("Mesh[%d] Primitive[%d] does not have key 'TANGENT'.", i, j);
                }

                if (attributes.find("TEXCOORD_0") != attributes.end())
                {
                    if (attributes["TEXCOORD_0"].is_int())
                    {
                        const i32 texcoord_index = attributes["TEXCOORD_0"].as_int();
                        p.texcoord_acessor = texcoord_index;
                    }
                    else
                    {
                        // JOJ_ERROR(ErrorCode::FAILED, "Mesh[%d] Primitive[%d] texcoord index is not an integer.", i, j);
                    }
                }
                else
                {
                    // JOJ_WARN("Mesh[%d] Primitive[%d] does not have key 'TEXCOORD_0'.", i, j);
                }

                if (attributes.find("COLOR_0") != attributes.end())
                {
                    if (attributes["COLOR_0"].is_int())
                    {
                        const i32 color_index = attributes["COLOR_0"].as_int();
                        p.color_acessor = color_index;
                    }
                    else
                    {
                        // JOJ_ERROR(ErrorCode::FAILED, "Mesh[%d] Primitive[%d] color index is not an integer.", i, j);
                    }
                }
                else
                {
                    // JOJ_WARN("Mesh[%d] Primitive[%d] does not have key 'COLOR_0'.", i, j);
                }

                if (attributes.find("JOINTS_0") != attributes.end())
                {
                    if (attributes["JOINTS_0"].is_int())
                    {
                        const i32 joint_index = attributes["JOINTS_0"].as_int();
                        p.joint_acessor = joint_index;
                    }
                    else
                    {
                        // JOJ_ERROR(ErrorCode::FAILED, "Mesh[%d] Primitive[%d] joint index is not an integer.", i, j);
                    }
                }
                else
                {
                    // JOJ_WARN("Mesh[%d] Primitive[%d] does not have key 'JOINTS_0'.", i, j);
                }

                if (attributes.find("WEIGHTS_0") != attributes.end())
                {
                    if (attributes["WEIGHTS_0"].is_int())
                    {
                        const i32 weight_index = attributes["WEIGHTS_0"].as_int();
                        p.weight_acessor = weight_index;
                    }
                    else
                    {
                        // JOJ_ERROR(ErrorCode::FAILED, "Mesh[%d] Primitive[%d] weight index is not an integer.", i, j);
                    }
                }
                else
                {
                    // JOJ_WARN("Mesh[%d] Primitive[%d] does not have key 'WEIGHTS_0'.", i, j);
                }
            }
            else
            {
                // JOJ_ERROR(ErrorCode::FAILED, "Mesh[%d] Primitive[%d] does not have key 'attributes'.", i, j);
            }

            if (primitive.has_key("indices"))
            {
                if (primitive["indices"].is_int())
                {
                    const i32 indices_index = primitive["indices"].as_int();
                    p.indices_acessor = indices_index;
                }
                else
                {
                    // JOJ_ERROR(ErrorCode::FAILED, "Mesh[%d] Primitive[%d] indices index is not an integer.", i, j);
                }
            }
            else
            {
                // JOJ_WARN("Mesh[%d] Primitive[%d] does not have key 'indices'.", i, j);
            }

            m.primitives.push_back(p);
            ++j;
        }

        m_meshes.push_back(m);
        ++i;
    }

    return true;
}

void joj::GLTFImporter::print_meshes()
{
    std::cout << "Total loaded meshes: " << m_meshes.size() << std::endl;

    i32 i = 0;
    for (const auto& mesh : m_meshes)
    {
        std::cout << "Mesh " << i << ": " << std::endl;
        std::cout << "    Name: " << mesh.name << std::endl;
        i32 j = 0;
        for (const auto& primitive : mesh.primitives)
        {
            std::cout << "    Primitive " << j << ": " << std::endl;
            std::cout << "        Position accessor (attr): " << primitive.position_acessor << std::endl;
            std::cout << "        Normal accessor (attr): " << primitive.normal_acessor << std::endl;
            std::cout << "        Tangent accessor (attr): " << primitive.tangent_acessor << std::endl;
            std::cout << "        Texcoord accessor (attr): " << primitive.texcoord_acessor << std::endl;
            std::cout << "        Color accessor (attr): " << primitive.color_acessor << std::endl;
            std::cout << "        Joint accessor (attr): " << primitive.joint_acessor << std::endl;
            std::cout << "        Weight accessor (attr): " << primitive.weight_acessor << std::endl;
            std::cout << "        Indices accessor: " << primitive.indices_acessor << std::endl;
            std::cout << "        Material index: " << primitive.material_index << std::endl;
            std::cout << "        Mode: " << primitive_mode_to_str(primitive.mode) << std::endl;
            ++j;
        }
        ++i;
    }
}

b8 joj::GLTFImporter::load_animations()
{
    if (!m_root.has_key("animations"))
        return true;

    auto animations = m_root["animations"].as_array();
    i32 i = 0;
    for (const auto& animation : animations)
    {
        GLTFAnimation anim;
        if (animation.has_key("name"))
        {
            if (animation["name"].is_string())
            {
                const std::string anim_name = animation["name"].as_string();
                anim.name = anim_name;
            }
            else
            {
                // JOJ_ERROR(ErrorCode::FAILED, "Animation[%d] name is not a string.", i);
            }
        }
        else
        {
            // JOJ_WARN("Animation[%d] does not have key 'name'.", i);
        }

        if (animation.has_key("samplers"))
        {
            auto samplers = animation["samplers"].as_array();
            i32 j = 0;
            for (const auto& sampler : samplers)
            {
                GLTFSampler gltf_sampler;
                if (sampler.has_key("input"))
                {
                    if (sampler["input"].is_int())
                    {
                        const i32 input_index = sampler["input"].as_int();
                        gltf_sampler.input = input_index;
                    }
                    else
                    {
                        // JOJ_ERROR(ErrorCode::FAILED, "Animation[%d] Sampler[%d] input index is not an integer.", i, j);
                    }
                }
                else
                {
                    // JOJ_WARN("Animation[%d] Sampler[%d] does not have key 'input'.", i, j);
                }

                if (sampler.has_key("output"))
                {
                    if (sampler["output"].is_int())
                    {
                        const i32 output_index = sampler["output"].as_int();
                        gltf_sampler.output = output_index;
                    }
                    else
                    {
                        // JOJ_ERROR(ErrorCode::FAILED, "Animation[%d] Sampler[%d] output index is not an integer.", i, j);
                    }
                }
                else
                {
                    // JOJ_WARN("Animation[%d] Sampler[%d] does not have key 'output'.", i, j);
                }

                if (sampler.has_key("interpolation"))
                {
                    if (sampler["interpolation"].is_string())
                    {
                        const std::string interpolation = sampler["interpolation"].as_string();
                        if (interpolation == "LINEAR")
                            gltf_sampler.interpolation = InterpolationType::LINEAR;
                        else if (interpolation == "STEP")
                            gltf_sampler.interpolation = InterpolationType::STEP;
                        else if (interpolation == "CUBICSPLINE")
                            gltf_sampler.interpolation = InterpolationType::CUBICSPLINE;
                        else
                            gltf_sampler.interpolation = InterpolationType::UNKNOWN;
                    }
                    else
                    {
                        // JOJ_ERROR(ErrorCode::FAILED, "Animation[%d] Sampler[%d] interpolation is not a string.", i, j);
                    }
                }
                else
                {
                    // JOJ_WARN("Animation[%d] Sampler[%d] does not have key 'interpolation'.", i, j);
                }

                anim.samplers.push_back(gltf_sampler);
                ++j;
            }
        }
        else
        {
            // JOJ_WARN("Animation[%d] does not have key 'samplers'.", i);
        }

        if (animation.has_key("channels"))
        {
            auto channels = animation["channels"].as_array();
            i32 k = 0;
            for (const auto& channel : channels)
            {
                GLTFAnimationChannel gltf_channel;
    
                if (channel.has_key("sampler"))
                {
                    if (channel["sampler"].is_int())
                    {
                        const i32 sampler_index = channel["sampler"].as_int();
                        gltf_channel.sampler = sampler_index;
                    }
                    else
                    {
                        // JOJ_ERROR(ErrorCode::FAILED, "Animation[%d] Channel[%d] sampler index is not an integer.", i, k);
                    }
                }
                else
                {
                    // JOJ_WARN("Animation[%d] Channel[%d] does not have key 'sampler'.", i, k);
                }

                if (channel.has_key("target"))
                {
                    if (channel["target"].is_object())
                    {
                        auto target = channel["target"].as_object();
                        if (target.find("node") != target.end())
                        {
                            if (target["node"].is_int())
                            {
                                const i32 node_index = target["node"].as_int();
                                gltf_channel.target_node = node_index;
                            }
                            else
                            {
                                // JOJ_ERROR(ErrorCode::FAILED, "Animation[%d] Channel[%d] target node index is not an integer.", i, k);
                            }
                        }
                        else
                        {
                            // JOJ_ERROR(ErrorCode::FAILED, "Animation[%d] Channel[%d] does not have key 'node'.", i, k);
                        }

                        if (target.find("path") != target.end())
                        {
                            if (target["path"].is_string())
                            {
                                const std::string target_path = target["path"].as_string();
                                if (target_path == "translation")
                                    gltf_channel.type = AnimationChannelType::TRANSLATION;
                                else if (target_path == "rotation")
                                    gltf_channel.type = AnimationChannelType::ROTATION;
                                else if (target_path == "scale")
                                    gltf_channel.type = AnimationChannelType::SCALE;
                                else
                                    gltf_channel.type = AnimationChannelType::UNKNOWN;
                            }
                            else
                            {
                                // JOJ_ERROR(ErrorCode::FAILED, "Animation[%d] Channel[%d] target path is not a string.", i, k);
                            }
                        }
                        else
                        {
                            // JOJ_ERROR(ErrorCode::FAILED, "Animation[%d] Channel[%d] does not have key 'path'.", i, k);
                        }
                    }
                    else
                    {
                        // JOJ_ERROR(ErrorCode::FAILED, "Animation[%d] Channel[%d] target index is not an integer.", i, k);
                    }
                }
                else
                {
                    // JOJ_ERROR(ErrorCode::FAILED, "Animation[%d] Channel[%d] does not have key 'target'.", i, k);
                }

                anim.channels.push_back(gltf_channel);
                ++k;
            }
        }
        else
        {
            // JOJ_WARN("Animation[%d] does not have key 'channels'.", i);
        }

        m_animations.push_back(anim);
        ++i;
    }

    return true;
}

void joj::GLTFImporter::print_animations()
{
    std::cout << "Total loaded animations: " << m_animations.size() << std::endl;

    i32 i = 0;
    for (const auto& anim : m_animations)
    {
        std::cout << "Animation " << i << ": " << std::endl;
        std::cout << "    Name: " << anim.name << std::endl;

        i32 j = 0;
        for (const auto& channel : anim.channels)
        {
            std::cout << "    Channel " << j << ": " << std::endl;
            std::cout << "        Sampler: " << channel.sampler << std::endl;
            std::cout << "        Target: " << channel.target_node << std::endl;
            std::cout << "        Path: " << animation_channel_type_to_string(channel.type) << std::endl;
            ++j;
        }

        i32 k = 0;
        for (const auto& sampler : anim.samplers)
        {
            std::cout << "    Sampler " << k << ": " << std::endl;
            std::cout << "        Input: " << sampler.input << std::endl;
            std::cout << "        Output: " << sampler.output << std::endl;
            std::cout << "        Interpolation: " << interpolation_type_to_string(sampler.interpolation) << std::endl;
            ++k;
        }
        ++i;
    }
}

b8 joj::GLTFImporter::load_skins()
{
    if (!m_root.has_key("skins"))
        return true;

    auto skins = m_root["skins"].as_array();
    i32 i = 0;
    for (const auto& skin : skins)
    {
        GLTFSkin s;

        if (skin.has_key("name"))
        {
            if (skin["name"].is_string())
            {
                const std::string skin_name = skin["name"].as_string();
                s.name = skin_name;
            }
            else
            {
                // JOJ_ERROR(ErrorCode::FAILED, "Skin[%d] name is not a string.", i);
            }
        }
        else
        {
            // JOJ_WARN("Skin[%d] does not have key 'name'.", i);
        }

        if (skin.has_key("inverseBindMatrices"))
        {
            if (skin["inverseBindMatrices"].is_int())
            {
                const i32 inverse_bind_matrices_index = skin["inverseBindMatrices"].as_int();
                s.inverse_bind_matrices_accessor_index = inverse_bind_matrices_index;
            }
            else
            {
                // JOJ_ERROR(ErrorCode::FAILED, "Skin[%d] inverse bind matrices index is not an integer.", i);
            }
        }
        else
        {
            // JOJ_WARN("Skin[%d] does not have key 'inverseBindMatrices'.", i);
        }

        if (skin.has_key("skeleton"))
        {
            if (skin["skeleton"].is_int())
            {
                const i32 skeleton_root_node_index = skin["skeleton"].as_int();
                s.skeleton_root_node_index = skeleton_root_node_index;
            }
            else
            {
                // JOJ_ERROR(ErrorCode::FAILED, "Skin[%d] skeleton root node index is not an integer.", i);
            }
        }
        else
        {
            // JOJ_WARN("Skin[%d] does not have key 'skeleton'.", i);
        }

        if (skin.has_key("joints"))
        {
            auto joints = skin["joints"].as_array();
            for (const auto& joint : joints)
            {
                if (joint.is_int())
                {
                    s.joint_indices.push_back(joint.as_int());
                }
                else
                {
                    // JOJ_ERROR(ErrorCode::FAILED, "Skin[%d] joint index is not an integer.", i);
                }
            }
        }
        else
        {
            // JOJ_WARN("Skin[%d] does not have key 'joints'.", i);
        }

        m_skins.push_back(s);
    }

    return true;
}

void joj::GLTFImporter::print_skins()
{
    std::cout << "Total loaded skins: " << m_skins.size() << std::endl;

    i32 i = 0;
    for (const auto& skin : m_skins)
    {
        std::cout << "Skin " << i << ": " << std::endl;
        std::cout << "    Name: " << skin.name << std::endl;
        std::cout << "    Inverse bind matrices accessor index: " << skin.inverse_bind_matrices_accessor_index << std::endl;
        std::cout << "    Skeleton root node index: " << skin.skeleton_root_node_index << std::endl;
        if (!skin.joint_indices.empty())
        {
            std::cout << "    Joint indices: ";
            for (const auto& joint : skin.joint_indices)
                std::cout << joint << " ";
            std::cout << std::endl;
        }
        ++i;
    }
}

b8 joj::GLTFImporter::load_scenes()
{
    if (!m_root.has_key("scenes"))
        return true;

    auto scenes = m_root["scenes"].as_array();
    i32 i = 0;
    for (const auto& scene : scenes)
    {
        GLTFScene s;
        if (scene.has_key("name"))
        {
            if (scene["name"].is_string())
            {
                const std::string scene_name = scene["name"].as_string();
                s.name = scene_name;
            }
            else
            {
                // JOJ_ERROR(ErrorCode::FAILED, "Scene[%d] name is not a string.", i);
            }
        }
        else
        {
            // JOJ_WARN("Scene[%d] does not have key 'name'.", i);
        }

        if (scene.has_key("nodes"))
        {
            auto nodes = scene["nodes"].as_array();
            for (const auto& node : nodes)
            {
                if (node.is_int())
                {
                    s.root_nodes.push_back(node.as_int());
                }
                else
                {
                    // JOJ_ERROR(ErrorCode::FAILED, "Scene[%d] root node index is not an integer.", i);
                }
            }
        }
        else
        {
            // JOJ_WARN("Scene[%d] does not have key 'nodes'.", i);
        }

        m_scenes.push_back(s);
    }

    return true;
}

void joj::GLTFImporter::print_scenes()
{
    std::cout << "Total loaded scenes: " << m_scenes.size() << std::endl;

    i32 i = 0;
    for (const auto& scene : m_scenes)
    {
        std::cout << "Scene " << i << ": " << std::endl;
        std::cout << "    Name: " << scene.name << std::endl;
        if (!scene.root_nodes.empty())
        {
            std::cout << "    Root nodes: ";
            for (const auto& node : scene.root_nodes)
                std::cout << node << " ";
            std::cout << std::endl;
        }
        ++i;
    }
}

void joj::GLTFImporter::get_vertices(std::vector<GLTFVertex>& vertices)
{
}

void joj::GLTFImporter::get_indices(std::vector<u16>& indices)
{
}

void joj::GLTFImporter::get_vertices_and_indices(std::vector<GLTFVertex>& vertices, std::vector<u16>& indices)
{
    for (const auto& mesh : m_meshes)
    {
        for (const auto& primitive : mesh.primitives)
        {
            // 1. Pegar os índices dos accessors
            i32 index_position_accessor = primitive.position_acessor;
            i32 index_normal_accessor = primitive.normal_acessor;
            i32 index_color_accessor = primitive.color_acessor;
            i32 index_indices_accessor = primitive.indices_acessor;

            if (index_position_accessor == -1 || index_normal_accessor == -1 || index_indices_accessor == -1)
                continue; // Se faltar algum dado essencial, pula a primitive

            // 2. Obter os accessors correspondentes
            const GLTFAccessor& position_accessor = m_accessors[index_position_accessor];
            const GLTFAccessor& normal_accessor = m_accessors[index_normal_accessor];
            const GLTFAccessor& indices_accessor = m_accessors[index_indices_accessor];

            // 3. Pegar os BufferViews dos accessors
            const GLTFBufferView& position_buffer_view = m_buffer_views[position_accessor.buffer_view];
            const GLTFBufferView& normal_buffer_view = m_buffer_views[normal_accessor.buffer_view];
            const GLTFBufferView& indices_buffer_view = m_buffer_views[indices_accessor.buffer_view];

            // 4. Obter os Buffers correspondentes
            const Buffer& position_buffer = m_buffers[position_buffer_view.buffer];
            const Buffer& normal_buffer = m_buffers[normal_buffer_view.buffer];
            const Buffer& indices_buffer = m_buffers[indices_buffer_view.buffer];

            // 5. Extrair os dados dos buffers
            std::vector<Vector3> positions = read_buffer<Vector3>(position_buffer, position_accessor, position_buffer_view);
            std::vector<Vector3> normals = read_buffer<Vector3>(normal_buffer, normal_accessor, normal_buffer_view);
            std::vector<u16> indices_data = read_buffer<u16>(indices_buffer, indices_accessor, indices_buffer_view);

            std::vector<Vector4> colors; // RGBA
            if (index_color_accessor != -1)
            {
                const GLTFAccessor& color_accessor = m_accessors[index_color_accessor];
                const GLTFBufferView& color_buffer_view = m_buffer_views[color_accessor.buffer_view];
                const Buffer& color_buffer = m_buffers[color_buffer_view.buffer];

                // Verificar se o dado é vec3 (RGB) ou vec4 (RGBA)
                if (color_accessor.data_type == DataType::VEC3)
                {
                    std::vector<Vector3> temp_colors = read_buffer<Vector3>(color_buffer, color_accessor, color_buffer_view);
                    for (const auto& c : temp_colors)
                        colors.push_back(Vector4(c.x, c.y, c.z, 1.0f)); // Adiciona Alpha = 1.0f
                }
                else if (color_accessor.data_type == DataType::VEC4)
                {
                    colors = read_buffer<Vector4>(color_buffer, color_accessor, color_buffer_view);
                }
            }

            // 6. Criar os vértices
            for (size_t i = 0; i < positions.size(); i++)
            {
                GLTFVertex vertex;
                vertex.pos = positions[i];
                vertex.color = (i < colors.size()) ? colors[i] : Vector4(1, 1, 1, 1);
                vertex.normal = (i < normals.size()) ? normals[i] : Vector3(0, 0, 1); // Normal padrão caso não exista
                vertices.push_back(vertex);
            }

            // 7. Adicionar os índices na lista final
            indices.insert(indices.end(), indices_data.begin(), indices_data.end());
        }
    }
}