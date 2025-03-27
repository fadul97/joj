#include "joj/resources/old_gltf_importer.h"

#include <fstream>
#include <sstream>
#include "joj/utils/json_parser.h"
#include "joj/core/math/jmath.h"
#include "joj/core/logger.h"
#include <algorithm>

void joj::apply_animation(OLDGLTFAnimation& animation, f32 time, OLDGLTFNode& node)
{
    for (auto& channel : animation.channels)
    {
        const std::vector<OLDGLTFKeyFrame>& keyframes = channel.keyframes;

        i32 k0 = 0;
        i32 k1 = 0;

        for (i32 i = 0; i < keyframes.size() - 1; ++i)
        {
            if (time >= keyframes[i].time && time <= keyframes[i + 1].time)
            {
                k0 = i;
                k1 = i + 1;
                break;
            }
        }

        f32 t = (time - keyframes[k0].time) / (keyframes[k1].time - keyframes[k0].time);
        Vector3 value = lerp(keyframes[k0].translation, keyframes[k1].translation, t);

        // Apply value in correct property of the node
        if (channel.path == "translation")
            node.SetPosition(value);
        else if (channel.path == "scale")
            node.SetScale(value);
    }
}

void joj::apply_all_animations(OLDGLTFAnimation& animation, f32 time, OLDGLTFNode& node, b8 loop)
{
    for (auto& channel : animation.channels)
    {
        const std::vector<OLDGLTFKeyFrame>& keyframes = channel.keyframes;

        // Se a animação deve ser em loop, ajustar o tempo
        if (loop) {
            if (time > keyframes.back().time) {
                time = fmod(time, keyframes.back().time);  // Voltar para o início quando o tempo ultrapassar o último keyframe
                // O tempo será "redefinido" para um valor entre 0 e o último keyframe
            }
        } else {
            // Se o loop está desativado, a animação deve parar no último keyframe
            if (time > keyframes.back().time) {
                time = keyframes.back().time;  // Se passou do último keyframe, ajustamos para o último keyframe
            }
        }

        // Encontrar os keyframes de interpolação
        i32 k0 = 0;
        i32 k1 = 0;
        for (i32 i = 0; i < keyframes.size() - 1; ++i)
        {
            if (time >= keyframes[i].time && time <= keyframes[i + 1].time)
            {
                k0 = i;
                k1 = i + 1;
                break;
            }
        }

        // Caso o tempo ultrapasse o último keyframe, não precisa interpolar, apenas usa o último valor
        if (k1 == keyframes.size() - 1) {
            k0 = k1;  // Se for o último keyframe, simplesmente usamos o k1
        }

        // Calcular o valor de t para interpolação (evitar divisão por zero)
        if (keyframes[k1].time != keyframes[k0].time) {
            f32 t = (time - keyframes[k0].time) / (keyframes[k1].time - keyframes[k0].time);
            // Limitar t para que esteja entre 0 e 1
            t = std::min(std::max(t, 0.0f), 1.0f);

            // Aplicar as transformações de acordo com o tipo de canal
            if (channel.path == "translation")
            {
                Vector3 value = lerp(keyframes[k0].translation, keyframes[k1].translation, t);
                node.SetPosition(value);
            }
            else if (channel.path == "scale")
            {
                Vector3 value = lerp(keyframes[k0].scale, keyframes[k1].scale, t);
                node.SetScale(value);
            }
            else if (channel.path == "rotation")
            {
                // Converter Vector4 para XMVECTOR (quaternion do DirectXMath)
                DirectX::XMVECTOR q0 = DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4*>(&keyframes[k0].rotation));
                DirectX::XMVECTOR q1 = DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4*>(&keyframes[k1].rotation));

                // Interpolar usando SLERP
                DirectX::XMVECTOR interpolatedRotation = DirectX::XMQuaternionSlerp(q0, q1, t);

                Vector4 vec4_interpolated_ration = Vector4(interpolatedRotation);
                node.SetRotation(vec4_interpolated_ration);
            }
        }
        else
        {
            // Caso não seja necessário interpolar, aplique o valor do último keyframe
            if (channel.path == "translation")
            {
                node.SetPosition(keyframes[k0].translation);
                std::cout << "Not interpolating translation" << std::endl;
            }
            else if (channel.path == "scale")
            {
                node.SetScale(keyframes[k0].scale);
            }
            else if (channel.path == "rotation")
            {
                // Converter Vector4 para XMVECTOR
                DirectX::XMVECTOR lastRotation = DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4*>(&keyframes.back().rotation));

                Vector4 vec4_interpolated_ration = Vector4(lastRotation);
                node.SetRotation(vec4_interpolated_ration);
            }
        }
    }
}


void joj::apply_all_animations_almost(OLDGLTFAnimation& animation, f32 time, OLDGLTFNode& node)
{
    for (auto& channel : animation.channels)
    {
        const std::vector<OLDGLTFKeyFrame>& keyframes = channel.keyframes;

        i32 k0 = 0;
        i32 k1 = 0;

        // Encontrar os keyframes de interpolação
        for (i32 i = 0; i < keyframes.size() - 1; ++i)
        {
            if (time >= keyframes[i].time && time <= keyframes[i + 1].time)
            {
                k0 = i;
                k1 = i + 1;
                break;
            }
        }

        // Caso o tempo ultrapasse o último keyframe, não precisa interpolar, apenas usa o último valor
        if (k1 == keyframes.size() - 1) {
            k0 = k1;  // Set k0 to the last keyframe if time is greater than the last keyframe time
        }

        // Calcular o valor de t para interpolação (evitar divisão por zero)
        if (keyframes[k1].time != keyframes[k0].time) {
            f32 t = (time - keyframes[k0].time) / (keyframes[k1].time - keyframes[k0].time);
            // Limitar t para que esteja entre 0 e 1
            t = std::min(std::max(t, 0.0f), 1.0f);

            // Aplicar as transformações de acordo com o tipo de canal
            if (channel.path == "translation")
            {
                Vector3 value = lerp(keyframes[k0].translation, keyframes[k1].translation, t);
                node.SetPosition(value);
            }
            else if (channel.path == "scale")
            {
                Vector3 value = lerp(keyframes[k0].scale, keyframes[k1].scale, t);
                node.SetScale(value);
            }
            else if (channel.path == "rotation")
            {
                // Se a rotação não for usada, pode ser ignorada por enquanto.
            }
        }
        else {
            // Caso não seja necessário interpolar, aplique o valor do último keyframe
            if (channel.path == "translation")
            {
                node.SetPosition(keyframes[k0].translation);
            }
            else if (channel.path == "scale")
            {
                node.SetScale(keyframes[k0].scale);
            }
            else if (channel.path == "rotation")
            {
                // Aplique a rotação, se necessário
            }
        }
    }
}



void joj::apply_all_animations_old(OLDGLTFAnimation& animation, f32 time, OLDGLTFNode& node)
{
    for (auto& channel : animation.channels)
    {
        const std::vector<OLDGLTFKeyFrame>& keyframes = channel.keyframes;

        i32 k0 = 0;
        i32 k1 = 0;

        for (i32 i = 0; i < keyframes.size() - 1; ++i)
        {
            if (time >= keyframes[i].time && time <= keyframes[i + 1].time)
            {
                k0 = i;
                k1 = i + 1;
                break;
            }
        }

        f32 t = (time - keyframes[k0].time) / (keyframes[k1].time - keyframes[k0].time);

        if (channel.path == "translation")
        {
            Vector3 value = lerp(keyframes[k0].translation, keyframes[k1].translation, t);
            node.SetPosition(value);
        }
        else if (channel.path == "scale")
        {
            Vector3 value = lerp(keyframes[k0].scale, keyframes[k1].scale, t);
            node.SetScale(value);
        }
        else if (channel.path == "rotation")
        {
            /*
            // Interpolação de rotação (quaternion)
            Vector4 q0(keyframes[k0].rotation.x, keyframes[k0].rotation.y, keyframes[k0].rotation.z, keyframes[k0].rotation.w);
            Vector4 q1(keyframes[k1].rotation.x, keyframes[k1].rotation.y, keyframes[k1].rotation.z, keyframes[k1].rotation.w);
            Vector4 interpolated_rotation = joj::slerp(q0, q1, t);  // Interpolação esférica de quatérnions
            
            node.SetRotation(Vector4(interpolated_rotation.x, interpolated_rotation.y, interpolated_rotation.z, interpolated_rotation.w));
            */
        }
    }
}


joj::OLDGLTFImporter::OLDGLTFImporter()
    : m_gltf_filename(nullptr), m_bin_filename(""),
    m_positions_byte_offset(-1), m_normals_byte_offset(-1), m_indices_byte_offset(-1),
    m_positions_count(-1), m_normals_count(-1), m_indices_count(-1),
    m_animations_byte_offset(-1), m_animations_count(-1),
    m_translation_byte_offset(-1), m_rotation_byte_offset(-1), m_scale_byte_offset(-1),
    m_translation_count(-1), m_rotation_count(-1), m_scale_count(-1)
{
}

joj::OLDGLTFImporter::OLDGLTFImporter(const char* filename)
    : m_gltf_filename(filename), m_bin_filename(""),
    m_positions_byte_offset(-1), m_normals_byte_offset(-1), m_indices_byte_offset(-1),
    m_positions_count(-1), m_normals_count(-1), m_indices_count(-1),
    m_animations_byte_offset(-1), m_animations_count(-1),
    m_translation_byte_offset(-1), m_rotation_byte_offset(-1), m_scale_byte_offset(-1),
    m_translation_count(-1), m_rotation_count(-1), m_scale_count(-1)
{
}

joj::OLDGLTFImporter::~OLDGLTFImporter()
{
}

joj::ErrorCode joj::OLDGLTFImporter::load()
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
                //std::cout << "Arquivo binário: " << m_bin_filename << std::endl;
                break;
            }
        }
    }

    if (m_bin_filename.size() > 0)
    {
        if (!load_binary_file())
            return ErrorCode::FAILED;
        
        // JOJ_DEBUG("Printing Root");
        // print_root();
        // print_animation_info();

        load_buffers();
        load_buffer_views();
        load_accessors();
        load_samplers();

        load_animations();
    }
    else
    {
        return ErrorCode::FAILED;
    }

    return ErrorCode::OK;
}

void joj::OLDGLTFImporter::load_accessors()
{
    if (!m_root.has_key("accessors"))
        return;

    auto accessors = m_root["accessors"].as_array();
    for (const auto& accessor : accessors)
    {
        OLDGLTFAccessor gltf_accessor;
        gltf_accessor.type = accessor["type"].as_string(); // Tipo de dado (ex: "VEC3", "SCALAR")
        gltf_accessor.componentType = accessor["componentType"].as_int(); // Tipo do componente (ex: 5126 para float)
        gltf_accessor.count = accessor["count"].as_int(); // Número de elementos
        gltf_accessor.bufferView = accessor["bufferView"].as_int(); // Índice do buffer view
        gltf_accessor.byteOffset = accessor.has_key("byteOffset") ? accessor["byteOffset"].as_int() : 0; // Offset em bytes

        m_accessors.push_back(gltf_accessor); // Adiciona o accessor ao vetor
    }
}

void joj::OLDGLTFImporter::load_buffer_views()
{
    if (!m_root.has_key("bufferViews"))
        return;

    auto buffer_views = m_root["bufferViews"].as_array();
    for (const auto& buffer_view : buffer_views)
    {
        OLDGLTFBufferView gltf_buffer_view;
        gltf_buffer_view.buffer = buffer_view["buffer"].as_int();          // Índice do buffer
        gltf_buffer_view.byteOffset = buffer_view.has_key("byteOffset") ? buffer_view["byteOffset"].as_int() : 0; // Offset em bytes
        gltf_buffer_view.byteLength = buffer_view["byteLength"].as_int();  // Comprimento dos dados
        gltf_buffer_view.byteStride = buffer_view.has_key("byteStride") ? buffer_view["byteStride"].as_int() : 0; // Stride (se necessário)

        m_bufferViews.push_back(gltf_buffer_view);  // Adiciona o buffer view ao vetor
    }
}

void joj::OLDGLTFImporter::load_samplers()
{
    if (!m_root.has_key("animations"))
        return;

    // Acessa o array de animações
    auto animations = m_root["animations"].as_array();

    // Itera sobre cada animação
    for (const auto& animation : animations)
    {
        if (animation.has_key("samplers"))
        {
            auto samplers = animation["samplers"].as_array(); // Acessa a chave "samplers" dentro da animação

            // Itera sobre cada sampler na animação
            for (const auto& sampler : samplers)
            {
                OLDGLTFSampler gltf_sampler;
                gltf_sampler.input = sampler["input"].as_int(); // Índice do acessador de entrada
                gltf_sampler.output = sampler["output"].as_int(); // Índice do acessador de saída
                gltf_sampler.interpolation = sampler["interpolation"].as_string(); // Tipo de interpolação

                m_samplers.push_back(gltf_sampler); // Adiciona o sampler ao vetor
            }
        }
    }
}


std::vector<f32> joj::OLDGLTFImporter::load_buffer_data(const OLDGLTFBufferView& buffer_view)
{
    std::vector<f32> data;

    // Obtém o buffer correspondente
    auto& buffer = m_buffers[buffer_view.buffer];

    // Verifica se o byteOffset e o byteLength são válidos
    if (buffer_view.byteOffset < 0 || buffer_view.byteOffset >= buffer.data.size())
    {
        std::cerr << "Erro: byteOffset " << buffer_view.byteOffset << " está fora dos limites do buffer de tamanho " << buffer.data.size() << std::endl;
        throw std::out_of_range("byteOffset está fora dos limites do buffer.");
    }

    // Calcular o início e o fim dos dados, com uma verificação para garantir que não ultrapasse o fim do buffer
    auto data_start = buffer.data.begin() + buffer_view.byteOffset;
    auto data_end = data_start + buffer_view.byteLength;

    if (data_end > buffer.data.end())
    {
        std::cerr << "Erro: byteLength " << buffer_view.byteLength << " excede o tamanho do buffer de tamanho " << buffer.data.size() << std::endl;
        throw std::out_of_range("byteLength excede o tamanho do buffer.");
    }

    // Copia os dados para o vetor
    data.insert(data.end(), data_start, data_end);

    return data;
}


std::vector<u8> joj::OLDGLTFImporter::load_binary_data(const std::string& uri)
{
    std::vector<u8> data;
    // Aqui você pode carregar os dados binários de um arquivo ou de uma string base64 (dependendo do formato GLTF)
    // Exemplo de carregamento a partir de um arquivo:
    std::ifstream file(uri, std::ios::binary);
    if (file.is_open())
    {
        file.seekg(0, std::ios::end);
        size_t size = file.tellg();
        std::cout << "File size: " << size << std::endl;
        file.seekg(0, std::ios::beg);
        data.resize(size);
        file.read(reinterpret_cast<char*>(data.data()), size);
    }
    return data;
}

void joj::OLDGLTFImporter::load_buffers()
{
    if (!m_root.has_key("buffers"))
        return;

    auto buffers = m_root["buffers"].as_array();
    for (const auto& buffer : buffers)
    {
        OLDGLTFBuffer gltf_buffer;
        std::string uri = buffer["uri"].as_string();  // URI do buffer (se existir)
        
        // Print the buffer URI
        std::string buffer_uri = "models/";
        buffer_uri += uri;
        std::cout << "Buffer URI: " << buffer_uri<< std::endl;

        // Aqui você pode carregar os dados binários do buffer a partir de um arquivo ou URI
        gltf_buffer.data = load_binary_data(buffer_uri); // Função que você precisará implementar para carregar os dados binários
        m_buffers.push_back(gltf_buffer);
    }
}

void joj::OLDGLTFImporter::load_animations()
{
    if (!m_root.has_key("animations"))
        return;

    auto animations = m_root["animations"].as_array();
    for (const auto& animation : animations)
    {
        OLDGLTFAnimation anim;
        anim.name = animation["name"].as_string(); // Nome da animação
        
        auto channels = animation["channels"].as_array();
        for (const auto& channel : channels)
        {
            OLDGLTFAnimationChannel animation_channel;
            animation_channel.path = channel["target"]["path"].as_string(); // Caminho (ex: "translation", "scale", "rotation")
            animation_channel.target_node_index = channel["target"]["node"].as_int(); // Índice do node alvo

            // Obter o sampler (índice do sampler)
            i32 sampler_index = channel["sampler"].as_int();
            OLDGLTFSampler sampler = m_samplers[sampler_index]; // Recupera o sampler baseado no índice
            
            // Obter os acessadores de entrada (input) e saída (output)
            auto input_accessor = m_accessors[sampler.input]; // Acessador de entrada (tempo)
            auto output_accessor = m_accessors[sampler.output]; // Acessador de saída (valores de animação)

            // Usar o buffer view para acessar os dados
            auto input_buffer_view = m_bufferViews[input_accessor.bufferView]; // Buffer view para os dados de entrada
            auto output_buffer_view = m_bufferViews[output_accessor.bufferView]; // Buffer view para os dados de saída

            // Aqui você deveria carregar os dados dos buffers a partir dos offsets e comprimentos
            // Exemplo (leitura de dados simples, você pode precisar de algo mais complexo dependendo da estrutura do seu buffer):
            auto input_data = load_buffer_data(input_buffer_view);
            auto output_data = load_buffer_data(output_buffer_view);

            // Gerar keyframes
            for (size_t i = 0; i < input_data.size(); ++i)
            {
                OLDGLTFKeyFrame keyframe;
                keyframe.time = input_data[i]; // Tempo da animação

                // Dependendo do caminho (path), carregar os valores de transformação (translation, scale, rotation)
                if (animation_channel.path == "translation")
                {
                    keyframe.translation = Vector3(
                        output_data[i * 3],    // x
                        output_data[i * 3 + 1],// y
                        output_data[i * 3 + 2] // z
                    );
                }
                else if (animation_channel.path == "scale")
                {
                    keyframe.scale = Vector3(
                        output_data[i * 3],    // x
                        output_data[i * 3 + 1],// y
                        output_data[i * 3 + 2] // z
                    );
                }
                else if (animation_channel.path == "rotation")
                {
                    keyframe.rotation = Vector3(
                        output_data[i * 3],    // x
                        output_data[i * 3 + 1],// y
                        output_data[i * 3 + 2] // z
                    );
                }

                // Adiciona o keyframe ao canal da animação
                animation_channel.keyframes.push_back(keyframe);
            }

            // Adiciona o canal à animação
            anim.channels.push_back(animation_channel);
        }

        // Adiciona a animação ao vetor de animações
        m_animations.push_back(anim);
    }
}

b8 joj::OLDGLTFImporter::load_binary_file()
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

    return true;
}

std::vector<joj::OLDGLTFVertex> joj::OLDGLTFImporter::get_vertices()
{
    std::vector<OLDGLTFVertex> vertices;
    return vertices;
}

std::vector<joj::OLDGLTFAnimation>& joj::OLDGLTFImporter::get_animations()
{
    return m_animations;
}

b8 joj::OLDGLTFImporter::parse_json()
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

void joj::OLDGLTFImporter::extract_vertices()
{
    // Extrai os vértices do arquivo glTF
}

void joj::OLDGLTFImporter::extract_accessors()
{
    // Extrai os accessors do arquivo glTF
}

void joj::OLDGLTFImporter::extract_buffer_views()
{
    // Extrai os buffer views do arquivo glTF
}

void joj::OLDGLTFImporter::print_root()
{
    // Imprime a raiz do arquivo glTF
    m_root.print();
}

void joj::OLDGLTFImporter::print_scene_info()
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

void joj::OLDGLTFImporter::print_node_info()
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

void joj::OLDGLTFImporter::print_mesh_info()
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

void joj::OLDGLTFImporter::print_vertex_data()
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
                            else if (m_normals_byte_offset == -1)
                            {
                                m_normals_byte_offset = byte_offset;  // Assumindo que o próximo é Normais
                                std::cout << "Normals Byte Offset: " << m_normals_byte_offset << std::endl;
                            }
                            else
                            {
                                // TODO: Fill other Vector3 information
                            }
                        }
                        else if (type == "VEC2")  // UVs
                        {
                            // Preencha m_uvs_byte_offset, se necessário
                        }
                        else if (type == "SCALAR")
                        {
                            if (m_indices_byte_offset == -1)
                            {
                                m_indices_byte_offset = byte_offset;
                                std::cout << "Indices Byte Offset: " << m_indices_byte_offset << std::endl;
                            }
                            else
                            {
                                // TODO: Fill other Scalar information
                            }
                        }
                    }

                    if (buffer_view.has_key("byteLength"))
                    {
                        // std::cout << "  Byte Length: " << buffer_view["byteLength"].as_int() << " bytes" << std::endl;
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
                    else if (m_normals_count == -1)  // Se m_normals_byte_offset estiver vazio
                    {
                        m_normals_count = count;
                        std::cout << "Normals Count: " << m_normals_count << std::endl;
                    }
                    else
                    {
                        // TODO: Fill other Vector3 information
                    }
                }
                else if (type == "SCALAR")  // Índices
                {
                    if (m_indices_count == -1)
                    {
                        m_indices_count = count;
                        std::cout << "Indices Count: " << m_indices_count << std::endl;
                    }
                    else
                    {
                        // TODO: Fill other Scalar information
                    }
                }
                else if (type == "VEC2")  // UVs
                {
                    // Preencha o contador de UVs, se necessário
                }
            }
        }
    }

    // Processar animações
    if (m_root.has_key("animations")) 
    {
        auto animations = m_root["animations"].as_array();
        for (const auto& animation : animations)
        {
            auto channels = animation["channels"].as_array();
            auto samplers = animation["samplers"].as_array();

            for (const auto& channel : channels)
            {
                if (!channel.has_key("sampler") || !channel.has_key("target")) 
                    continue;

                i32 sampler_index = channel["sampler"].as_int();
                std::string path = channel["target"]["path"].as_string();

                if (sampler_index >= samplers.size()) 
                    continue; // Evita indexação inválida

                auto& sampler = samplers[sampler_index];

                if (!sampler.has_key("output")) 
                    continue;

                i32 output_accessor_index = sampler["output"].as_int();

                auto accessors = m_root["accessors"].as_array();
                if (output_accessor_index >= accessors.size()) 
                    continue;

                auto& output_accessor = accessors[output_accessor_index];

                if (!output_accessor.has_key("bufferView")) 
                    continue;

                i32 buffer_view_index = output_accessor["bufferView"].as_int();
                i32 count = output_accessor["count"].as_int();

                auto buffer_views = m_root["bufferViews"].as_array();
                if (buffer_view_index >= buffer_views.size()) 
                    continue;

                auto& buffer_view = buffer_views[buffer_view_index];

                if (!buffer_view.has_key("byteOffset")) 
                    continue;

                i32 byte_offset = buffer_view["byteOffset"].as_int();

                if (path == "translation")
                {
                    m_translation_byte_offset = byte_offset;
                    m_translation_count = count;
                    // std::cout << "Translation Byte Offset: " << m_translation_byte_offset << "\n";
                    // std::cout << "Translation Count: " << m_translation_count << "\n";
                }
                else if (path == "rotation")
                {
                    m_rotation_byte_offset = byte_offset;
                    m_rotation_count = count;
                    // std::cout << "Rotation Byte Offset: " << m_rotation_byte_offset << "\n";
                    // std::cout << "Rotation Count: " << m_rotation_count << "\n";
                }
                else if (path == "scale")
                {
                    m_scale_byte_offset = byte_offset;
                    m_scale_count = count;
                    // std::cout << "Scale Byte Offset: " << m_scale_byte_offset << "\n";
                    // std::cout << "Scale Count: " << m_scale_count << "\n";
                }
            }
        }
    }


    load_positions_from_buffer(m_positions_byte_offset, m_positions_count);
    load_normals_from_buffer(m_normals_byte_offset, m_normals_count);
    load_indices_from_buffer(m_indices_byte_offset, m_indices_count);
    // load_animations_from_buffer(m_animations_byte_offset, m_animations_count);
    load_translation_from_buffer(m_translation_byte_offset, m_translation_count);
    load_rotation_from_buffer(m_rotation_byte_offset, m_rotation_count);
    load_scale_from_buffer(m_scale_byte_offset, m_scale_count);
}

void joj::OLDGLTFImporter::print_animation_info()
{
    if (!m_root.has_key("animations"))
    {
        std::cout << "No animations found in the JSON file.\n";
        return;
    }

    auto animations = m_root["animations"].as_array();

    for (size_t i = 0; i < animations.size(); ++i)
    {
        const auto& animation = animations[i];

        std::cout << "Animation " << i + 1 << " - Name: " << animation["name"].as_string() << "\n";

        // Imprime canais
        if (animation.has_key("channels"))
        {
            auto channels = animation["channels"].as_array();
            std::cout << "  Channels: \n";
            for (size_t j = 0; j < channels.size(); ++j)
            {
                const auto& channel = channels[j];
                std::cout << "    Channel " << j + 1 << " - Path: " << channel["target"]["path"].as_string()
                          << ", Node: " << channel["target"]["node"].as_int() << ", Sampler: " << channel["sampler"].as_int() << "\n";
            }
        }
        else
        {
            std::cout << "  No channels found for this animation.\n";
        }

        // Imprime samplers
        if (animation.has_key("samplers"))
        {
            auto samplers = animation["samplers"].as_array();
            std::cout << "  Samplers: \n";
            for (size_t j = 0; j < samplers.size(); ++j)
            {
                const auto& sampler = samplers[j];
                std::cout << "    Sampler " << j + 1 << " - Input: " << sampler["input"].as_int()
                          << ", Output: " << sampler["output"].as_int()
                          << ", Interpolation: " << sampler["interpolation"].as_string() << "\n";
            }
        }
        else
        {
            std::cout << "  No samplers found for this animation.\n";
        }

        // Tente imprimir toda a estrutura de keyframes
        if (animation.has_key("keyframes"))
        {
            auto keyframes = animation["keyframes"].as_array();
            std::cout << "  Keyframes found, inspecting data...\n";
            for (size_t k = 0; k < keyframes.size(); ++k)
            {
                const auto& keyframe = keyframes[k];
                std::cout << "    Keyframe " << k + 1 << " - Time: " << keyframe["time"].as_number() << "\n";
                
                // Tente imprimir os dados de translation, rotation, scale
                if (keyframe.has_key("translation"))
                {
                    auto translation = keyframe["translation"].as_array();
                    std::cout << "      Translation: (" 
                        << translation[0].as_number() << ", "
                        << translation[1].as_number() << ", "
                        << translation[2].as_number() << ")\n";
                }
                else
                {
                    std::cout << "      No translation found for this keyframe.\n";
                }

                if (keyframe.has_key("rotation"))
                {
                    auto rotation = keyframe["rotation"].as_array();
                    std::cout << "      Rotation: (" 
                        << rotation[0].as_number() << ", "
                        << rotation[1].as_number() << ", "
                        << rotation[2].as_number() << ")\n";
                }
                else
                {
                    std::cout << "      No rotation found for this keyframe.\n";
                }

                if (keyframe.has_key("scale"))
                {
                    auto scale = keyframe["scale"].as_array();
                    std::cout << "      Scale: (" 
                        << scale[0].as_number() << ", "
                        << scale[1].as_number() << ", "
                        << scale[2].as_number() << ")\n";
                }
                else
                {
                    std::cout << "      No scale found for this keyframe.\n";
                }
            }
        }
        else
        {
            std::cout << "  No keyframes found for this animation.\n";
        }
    }
}

// Função de comparação para ordenar os keyframes pelo tempo
bool compareKeyframes(const joj::OLDGLTFKeyFrame& a, const joj::OLDGLTFKeyFrame& b)
{
    return a.time < b.time; // Ordena de forma crescente pelo valor de 'time'
}

void joj::OLDGLTFImporter::print_animation_data()
{
    // Verificar se há animações
    if (m_animations.empty())
    {
        std::cout << "Nenhuma animação encontrada!" << std::endl;
        return;
    }

    // Iterar sobre cada animação
    for (size_t animIndex = 0; animIndex < m_animations.size(); ++animIndex)
    {
        const OLDGLTFAnimation& animation = m_animations[animIndex];

        // Imprimir nome da animação
        std::cout << "Animação #" << animIndex + 1 << ": " << animation.name << std::endl;

        // Iterar sobre os canais de animação da animação
        for (size_t channelIndex = 0; channelIndex < animation.channels.size(); ++channelIndex)
        {
            const OLDGLTFAnimationChannel& channel = animation.channels[channelIndex];

            // Imprimir caminho (path) do canal e índice do nó alvo
            std::cout << "  Canal #" << channelIndex + 1 << ": " << channel.path
                      << " (Nó alvo: " << channel.target_node_index << ")" << std::endl;

            // Iterar sobre os keyframes do canal
            for (size_t keyframeIndex = 0; keyframeIndex < channel.keyframes.size(); ++keyframeIndex)
            {
                const OLDGLTFKeyFrame& keyframe = channel.keyframes[keyframeIndex];

                // Imprimir dados do keyframe
                std::cout << "    Keyframe #" << keyframeIndex + 1
                          << ": Tempo = " << keyframe.time
                          << ", Tradução = (" << keyframe.translation.x << ", " << keyframe.translation.y << ", " << keyframe.translation.z << ")"
                          << ", Escala = (" << keyframe.scale.x << ", " << keyframe.scale.y << ", " << keyframe.scale.z << ")"
                          << ", Rotação = (" << keyframe.rotation.x << ", " << keyframe.rotation.y << ", " << keyframe.rotation.z << ")" << std::endl;
            }
        }
    }
}


void joj::OLDGLTFImporter::load_positions_from_buffer(const size_t byte_offset, const size_t count)
{
    const joj::Vector3* data = reinterpret_cast<const joj::Vector3*>(m_binary_data.data() + byte_offset);
    m_positions = std::vector<joj::Vector3>(data, data + count);
}

void joj::OLDGLTFImporter::load_normals_from_buffer(const size_t byte_offset, const size_t count)
{
    const joj::Vector3* data = reinterpret_cast<const joj::Vector3*>(m_binary_data.data() + byte_offset);
    m_normals = std::vector<joj::Vector3>(data, data + count);
}

void joj::OLDGLTFImporter::load_indices_from_buffer(const size_t byte_offset, const size_t count)
{
    const u16* data = reinterpret_cast<const u16*>(m_binary_data.data() + byte_offset);
    m_indices = std::vector<u16>(data, data + count);
}

void joj::OLDGLTFImporter::load_animations_from_buffer(const size_t byte_offset, const size_t count)
{
    const f32* data = reinterpret_cast<const f32*>(m_binary_data.data() + byte_offset);

    // Supondo que os keyframes estejam organizados em sequência:
    // Primeiro um array de tempos (SCALAR)
    // Depois arrays de valores correspondentes (VEC3 para translação/escala, VEC4 para rotação)

    std::vector<f32> keyframe_times(data, data + count); // Array de tempos

    // Aqui precisaríamos de mais contexto para saber como os dados estão organizados.
    // Para simplificação, assumo que temos 3 canais fixos: "translation", "rotation", "scale".

    // Os buffers de translação, rotação e escala vêm em sequência após os tempos
    const Vector3* translations = reinterpret_cast<const Vector3*>(data + count);
    const Vector4* rotations = reinterpret_cast<const Vector4*>(translations + count);
    const Vector3* scales = reinterpret_cast<const Vector3*>(rotations + count);

    // Criamos uma animação
    OLDGLTFAnimation animation;
    animation.name = "CubeAnimation"; // Nome genérico

    // Criamos os canais
    OLDGLTFAnimationChannel translation_channel;
    translation_channel.path = "translation";
    translation_channel.target_node_index = 0;

    OLDGLTFAnimationChannel rotation_channel;
    rotation_channel.path = "rotation";
    rotation_channel.target_node_index = 0;

    OLDGLTFAnimationChannel scale_channel;
    scale_channel.path = "scale";
    scale_channel.target_node_index = 0;

    // Preenchendo keyframes nos canais corretos
    for (size_t i = 0; i < count; i++)
    {
        OLDGLTFKeyFrame keyframe;
        keyframe.time = keyframe_times[i];
        keyframe.translation = translations[i];
        keyframe.rotation = { rotations[i].x, rotations[i].y, rotations[i].z }; // Apenas XYZ para simplificar
        keyframe.scale = scales[i];

        translation_channel.keyframes.push_back({ keyframe.time, keyframe.translation });
        rotation_channel.keyframes.push_back({ keyframe.time, keyframe.rotation });
        scale_channel.keyframes.push_back({ keyframe.time, keyframe.scale });
    }

    // Adicionamos os canais na animação
    animation.channels.push_back(translation_channel);
    animation.channels.push_back(rotation_channel);
    animation.channels.push_back(scale_channel);

    // Adicionamos essa animação ao conjunto de animações carregadas
    m_animations.push_back(animation);
}

void joj::OLDGLTFImporter::load_translation_from_buffer(const size_t byte_offset, const size_t count)
{
    const joj::Vector3* data = reinterpret_cast<const joj::Vector3*>(m_binary_data.data() + byte_offset);
    m_translations = std::vector<joj::Vector3>(data, data + count);
}

void joj::OLDGLTFImporter::load_rotation_from_buffer(const size_t byte_offset, const size_t count)
{
    const float* data = reinterpret_cast<const float*>(m_binary_data.data() + byte_offset);
    m_rotations.reserve(count);
    
    for (size_t i = 0; i < count; ++i)
    {
        Vector4 quat(
            data[i * 4 + 0], // x
            data[i * 4 + 1], // y
            data[i * 4 + 2], // z
            data[i * 4 + 3]  // w
        );
        m_rotations.push_back(quat);
    }
}

void joj::OLDGLTFImporter::load_scale_from_buffer(const size_t byte_offset, const size_t count)
{
    const joj::Vector3* data = reinterpret_cast<const joj::Vector3*>(m_binary_data.data() + byte_offset);
    m_scales = std::vector<joj::Vector3>(data, data + count);
}

void joj::OLDGLTFImporter::setup_animations()
{
    m_animations.clear();

    OLDGLTFAnimation animation;
    animation.name = "SimpleAnimation";  // Nome da animação

    // Vamos assumir que cada dado de animação (translation, rotation, scale) está mapeado para um único canal
    // e que a animação está sendo aplicada ao nó com índice 0, mas você pode expandir isso conforme a estrutura do seu arquivo GLTF

    // Definir o canal de "translation"
    OLDGLTFAnimationChannel translationChannel;
    translationChannel.path = "translation";  // Nome do caminho (path) da transformação
    translationChannel.target_node_index = 0;  // O nó alvo que está sendo animado, pode ser o cubo, por exemplo

    // Preencher os keyframes de tradução
    for (size_t i = 0; i < m_translations.size(); ++i)
    {
        OLDGLTFKeyFrame keyframe;
        keyframe.time = static_cast<f32>(i);  // Tempo da animação (ou pode ser baseado em algum fator de tempo real)
        keyframe.translation = m_translations[i];
        keyframe.scale = Vector3(1.0f, 1.0f, 1.0f);  // Se não tiver animação de escala, podemos deixar o valor fixo
        keyframe.rotation = Vector3(0.0f, 0.0f, 0.0f);  // Se não tiver animação de rotação, podemos deixar fixo

        translationChannel.keyframes.push_back(keyframe);
    }

    // Definir o canal de "rotation"
    OLDGLTFAnimationChannel rotationChannel;
    rotationChannel.path = "rotation";
    rotationChannel.target_node_index = 0;  // Novamente, aplicando ao nó com índice 0 (como exemplo)

    // Preencher os keyframes de rotação
    for (size_t i = 0; i < m_rotations.size(); ++i)
    {
        OLDGLTFKeyFrame keyframe;
        keyframe.time = static_cast<f32>(i);  // Tempo da animação
        keyframe.translation = Vector3(0.0f, 0.0f, 0.0f);  // Não alteramos a tradução aqui
        keyframe.scale = Vector3(1.0f, 1.0f, 1.0f);  // Não alteramos a escala
        keyframe.rotation = Vector3{ m_rotations[i].x, m_rotations[i].y, m_rotations[i].z };  // Usando o vetor de rotações

        rotationChannel.keyframes.push_back(keyframe);
    }

    // Definir o canal de "scale"
    OLDGLTFAnimationChannel scaleChannel;
    scaleChannel.path = "scale";
    scaleChannel.target_node_index = 0;  // Novamente, aplicando ao nó com índice 0

    // Preencher os keyframes de escala
    for (size_t i = 0; i < m_scales.size(); ++i)
    {
        OLDGLTFKeyFrame keyframe;
        keyframe.time = static_cast<f32>(i);  // Tempo da animação
        keyframe.translation = Vector3(0.0f, 0.0f, 0.0f);  // Não alteramos a tradução
        keyframe.scale = m_scales[i];  // Usando o vetor de escalas
        keyframe.rotation = Vector3(0.0f, 0.0f, 0.0f);  // Não alteramos a rotação

        scaleChannel.keyframes.push_back(keyframe);
    }

    // Adicionar os canais à animação
    animation.channels.push_back(translationChannel);
    animation.channels.push_back(rotationChannel);
    animation.channels.push_back(scaleChannel);

    // Adicionar a animação à lista de animações
    m_animations.push_back(animation);
}

void joj::OLDGLTFImporter::print_translation_data()
{
    u32 count = 0;
    for (const auto& translation : m_translations)
    {
        std::cout << "Translation " << count++ << ": (" << translation.x << ", " << translation.y << ", " << translation.z << ")" << std::endl;
    }
}

void joj::OLDGLTFImporter::print_rotation_data()
{
    u32 count = 0;
    for (const auto& rotation : m_rotations)
    {
        std::cout << "Rotation " << count++ << ": (" << rotation.x << ", " << rotation.y << ", " << rotation.z << ", " << rotation.w << ")" << std::endl;
    }
}

void joj::OLDGLTFImporter::print_scale_data()
{
    u32 count = 0;
    for (const auto& scale : m_scales)
    {
        std::cout << "Scale " << count++ << ": (" << scale.x << ", " << scale.y << ", " << scale.z << ")" << std::endl;
    }
}