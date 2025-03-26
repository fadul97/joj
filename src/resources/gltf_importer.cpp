#include "joj/resources/gltf_importer.h"

#include <fstream>
#include <sstream>
#include "joj/utils/json_parser.h"

joj::GLTFImporter::GLTFImporter()
    : m_gltf_filename(nullptr), m_bin_filename(""),
    m_positions_byte_offset(-1), m_normals_byte_offset(-1), m_indices_byte_offset(-1),
    m_positions_count(-1), m_normals_count(-1), m_indices_count(-1)
{
}

joj::GLTFImporter::GLTFImporter(const char* filename)
    : m_gltf_filename(filename), m_bin_filename(""),
    m_positions_byte_offset(-1), m_normals_byte_offset(-1), m_indices_byte_offset(-1),
    m_positions_count(-1), m_normals_count(-1), m_indices_count(-1)
{
}

joj::GLTFImporter::~GLTFImporter()
{
}

joj::ErrorCode joj::GLTFImporter::load()
{
    if (m_gltf_filename == nullptr)
        return ErrorCode::FAILED;
    
    std::ifstream file(m_gltf_filename);
    if (!file.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo: " << m_gltf_filename << std::endl;
        return ErrorCode::FAILED;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string json_content = buffer.str();
    JsonParser parser(json_content);
    m_root = parser.parse();

    if (parser.get_error_count() > 0)
    {
        std::cerr << "Erro ao parsear o arquivo glTF." << std::endl;
        return ErrorCode::FAILED;
    }

    // Get the binary file name and print it
    if (m_root.has_key("buffers"))
    {
        auto buffers = m_root["buffers"].as_array();
        for (const auto& buffer : buffers)
        {
            if (buffer.has_key("uri"))
            {
                m_bin_filename = "models/";
                m_bin_filename += buffer["uri"].as_string().c_str();
                std::cout << "Arquivo binário: " << m_bin_filename << std::endl;
                break;
            }
        }
    }

    if (m_bin_filename.size() > 0)
    {
        if (!load_binary_file())
            return ErrorCode::FAILED;
    }
    else
    {
        return ErrorCode::FAILED;
    }

    return ErrorCode::OK;
}

b8 joj::GLTFImporter::load_binary_file()
{
    std::ifstream file(m_bin_filename.c_str(), std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo binário: " << m_bin_filename << std::endl;
        return false;
    }

    file.seekg(0, std::ios::end); // Go to the end of the file
    size_t size = file.tellg(); // Get the file size
    std::cout << "File size: " << size << std::endl;
    file.seekg(0, std::ios::beg); // Go back to the beginning of the file

    m_binary_data = std::vector<u8>(size);
    file.read(reinterpret_cast<char*>(m_binary_data.data()), size);
}

std::vector<joj::GLTFVertex> joj::GLTFImporter::get_vertices()
{
    std::vector<GLTFVertex> vertices;
    return vertices;
}

b8 joj::GLTFImporter::parse_json()
{
    // Carrega e analisa o arquivo JSON (o arquivo glTF)
    std::ifstream file(m_gltf_filename);
    if (!file.is_open())
        return false;

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string json_content = buffer.str();

    // Faz o parsing do conteúdo JSON
    JsonParser parser(json_content);
    m_root = parser.parse();  // Assume que o JsonParser retorna a raiz do JSON.

    // Extração de dados relevantes:
    extract_vertices();
    extract_accessors();
    extract_buffer_views();

    return true;
}

void joj::GLTFImporter::extract_vertices()
{
    // Extrai os vértices do arquivo glTF
}

void joj::GLTFImporter::extract_accessors()
{
    // Extrai os accessors do arquivo glTF
}

void joj::GLTFImporter::extract_buffer_views()
{
    // Extrai os buffer views do arquivo glTF
}

void joj::GLTFImporter::print_scene_info()
{
    // Acessa a chave "scenes"
    if (m_root.has_key("scenes"))
    {
        auto scenes = m_root["scenes"].as_array();
        for (const auto& scene : scenes)
        {
            if (scene.has_key("name"))
            {
                std::cout << "Cena: " << scene["name"].as_string() << std::endl;
            }

            if (scene.has_key("nodes"))
            {
                auto nodes = scene["nodes"].as_array();
                for (size_t i = 0; i < nodes.size(); ++i)
                {
                    std::cout << "  Nó: " << nodes[i].as_int() << std::endl;
                }
            }
        }
    }
}

void joj::GLTFImporter::print_node_info()
{
    // Acessa a chave "nodes"
    if (m_root.has_key("nodes"))
    {
        auto nodes = m_root["nodes"].as_array();
        for (const auto& node : nodes)
        {
            if (node.has_key("name"))
            {
                std::cout << "Nome do nó: " << node["name"].as_string() << std::endl;
            }

            if (node.has_key("mesh"))
            {
                std::cout << "  Malha: " << node["mesh"].as_int() << std::endl;
            }

            if (node.has_key("translation"))
            {
                auto translation = node["translation"].as_array();
                std::cout << "  Tradução: ";
                for (const auto& t : translation)
                {
                    std::cout << t.as_number() << " ";
                }
                std::cout << std::endl;
            }
        }
    }
}

void joj::GLTFImporter::print_mesh_info()
{
    // Acessa a chave "meshes"
    if (m_root.has_key("meshes"))
    {
        auto meshes = m_root["meshes"].as_array();
        for (const auto& mesh : meshes)
        {
            if (mesh.has_key("primitives"))
            {
                auto primitives = mesh["primitives"].as_array();
                for (const auto& primitive : primitives)
                {
                    std::cout << "  Primitive: " << std::endl;

                    if (primitive.has_key("attributes"))
                    {
                        auto attributes = primitive["attributes"].as_object();
                        for (const auto& attribute : attributes)
                        {
                            std::cout << "    Atributo: " << attribute.first << " -> " << attribute.second.as_int() << std::endl;
                        }
                    }

                    if (primitive.has_key("indices"))
                    {
                        std::cout << "  Índices: " << primitive["indices"].as_int() << std::endl;
                    }
                }
            }
        }
    }
}

void joj::GLTFImporter::print_vertex_data()
{
    // Check if the root has key "accessors"
    if (m_root.has_key("accessors"))
    {
        // Access the accessors array
        auto accessors = m_root["accessors"].as_array();
        for (const auto& accessor : accessors)
        {
            // Check if the accessor has key "type"
            std::string type = accessor.has_key("type") ? accessor["type"].as_string() : "";

            // Check if the accessor has key "bufferView"
            if (accessor.has_key("bufferView"))
            {
                i32 buffer_view_index = accessor["bufferView"].as_int();
                std::cout << "BufferView: " << buffer_view_index << std::endl;

                if (m_root.has_key("bufferViews"))
                {
                    auto buffer_views = m_root["bufferViews"].as_array();
                    const auto& buffer_view = buffer_views[buffer_view_index];

                    // ByteOffset, ByteLength
                    if (buffer_view.has_key("byteOffset"))
                    {
                        i32 byte_offset = buffer_view["byteOffset"].as_int();
                        std::cout << "  Byte Offset: " << byte_offset << " bytes" << std::endl;

                        // Verifique o tipo para associar o byteOffset
                        if (type == "VEC3")  // Posições ou Normais
                        {
                            if (m_positions_byte_offset == -1)  // Verifique se a variável não foi preenchida
                            {
                                m_positions_byte_offset = byte_offset;
                                std::cout << "Positions Byte Offset: " << m_positions_byte_offset << std::endl;
                            }
                            else
                            {
                                m_normals_byte_offset = byte_offset;  // Assumindo que o próximo é Normais
                                std::cout << "Normals Byte Offset: " << m_normals_byte_offset << std::endl;
                            }
                        }
                        else if (type == "VEC2")  // UVs
                        {
                            // Preencha m_uvs_byte_offset, se necessário
                        }
                        else if (type == "SCALAR")
                        {
                            m_indices_byte_offset = byte_offset;
                            std::cout << "Indices Byte Offset: " << m_indices_byte_offset << std::endl;
                        }
                    }

                    if (buffer_view.has_key("byteLength"))
                    {
                        std::cout << "  Byte Length: " << buffer_view["byteLength"].as_int() << " bytes" << std::endl;
                    }
                }
            }

            // Preenche os dados de índice
            if (accessor.has_key("componentType"))
            {
                std::string component_type = accessor["componentType"].as_string();
                std::cout << "  Component Type: " << component_type << std::endl;
            }

            // Preenche o número de vértices ou índices
            if (accessor.has_key("count"))
            {
                int count = accessor["count"].as_int();
                if (type == "VEC3")  // Posições ou Normais
                {
                    if (m_positions_count == -1)  // Se m_positions_byte_offset estiver vazio
                    {
                        m_positions_count = count;
                        std::cout << "Positions Count: " << m_positions_count << std::endl;
                    }
                    else
                    {
                        m_normals_count = count;
                        std::cout << "Normals Count: " << m_normals_count << std::endl;
                    }
                }
                else if (type == "SCALAR")  // Índices
                {
                    m_indices_count = count;
                    std::cout << "Indices Count: " << m_indices_count << std::endl;
                }
                else if (type == "VEC2")  // UVs
                {
                    // Preencha o contador de UVs, se necessário
                }
            }
        }
    }

    load_positions_from_buffer(m_positions_byte_offset, m_positions_count);
    load_normals_from_buffer(m_normals_byte_offset, m_normals_count);
    load_indices_from_buffer(m_indices_byte_offset, m_indices_count);
}

void joj::GLTFImporter::load_positions_from_buffer(const size_t byte_offset, const size_t count)
{
    const joj::Vector3* data = reinterpret_cast<const joj::Vector3*>(m_binary_data.data() + byte_offset);
    m_positions = std::vector<joj::Vector3>(data, data + count);
}

void joj::GLTFImporter::load_normals_from_buffer(const size_t byte_offset, const size_t count)
{
    const joj::Vector3* data = reinterpret_cast<const joj::Vector3*>(m_binary_data.data() + byte_offset);
    m_normals = std::vector<joj::Vector3>(data, data + count);
}

void joj::GLTFImporter::load_indices_from_buffer(const size_t byte_offset, const size_t count)
{
    const u16* data = reinterpret_cast<const u16*>(m_binary_data.data() + byte_offset);
    m_indices = std::vector<u16>(data, data + count);
}