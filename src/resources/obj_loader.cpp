#include "resources/obj_loader.h"

#include "core/error_code.h"
#include "core/logger.h"
#include <fstream>
#include <unordered_map>
#include <fstream>
#include <string>
#include <sstream>
#include "renderer/vertex.h"

joj::OBJLoader::OBJLoader()
{
}

joj::OBJLoader::~OBJLoader()
{
    for (const auto mesh : m_meshes)
    {
        delete mesh;
    }
}

joj::InternalMesh* joj::OBJLoader::load(const char* filename)
{
    std::ifstream fin(filename);
    if (!fin.is_open())
    {
        JERROR(joj::ErrorCode::ERR_FILE_OBJ_OPENED_FAILED,
            "Failed to open file '%s'.", filename);
        return nullptr;
    }

    std::vector<JVector3> temp_positions;
    std::vector<JVector3> temp_normals;
    std::vector<JFloat2> temp_texcoords;

    std::unordered_map<std::string, u32> index_map;
    std::string line;

    InternalMesh* mesh = new InternalMesh();

    while (std::getline(fin, line))
    {
        std::istringstream iss(line);

        // Identify line prefix
        std::string prefix;
        iss >> prefix;

        // Vertices
        if (prefix == "v")
        {
            JVector3 position;
            iss >> position.x >> position.y >> position.z;
            temp_positions.push_back(position);
        }
        // Normals
        else if (prefix == "vn")
        {
            JVector3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
        }
        // Texture coordinates
        else if (prefix == "vt")
        {
            JFloat2 texcoord;
            iss >> texcoord.x >> texcoord.y;
            temp_texcoords.push_back(texcoord);
        }
        // Faces
        else if (prefix == "f")
        {
            std::string vertex_data;
            // Read every 1 face (3 indices)
            for (i32 i = 0; i < 3; ++i)
            {
                iss >> vertex_data;
                std::istringstream vertexStream(vertex_data);
                std::string v, t, n;
                u32 posIndex = 0, texIndex = 0, normIndex = 0;

                // Read vertice index
                std::getline(vertexStream, v, '/');
                if (!v.empty()) posIndex = std::stoi(v) - 1;

                // Read texture coordinate index
                if (std::getline(vertexStream, t, '/'))
                {
                    if (!t.empty()) texIndex = std::stoi(t) - 1;
                }

                // Read normal index
                if (std::getline(vertexStream, n))
                {
                    if (!n.empty()) normIndex = std::stoi(n) - 1;
                }

                // Not using Texture Coordinates
                joj::Vertex::PosColorNormal vertex = {};
                if (posIndex < temp_positions.size())
                    vertex.pos = temp_positions[posIndex];
                if (normIndex < temp_normals.size())
                    vertex.normal = temp_normals[normIndex];

                if (texIndex < temp_texcoords.size())
                {
                    // TODO: Set texture coordinates
                }

                // Check if vertice was already added, if yes, use index
                auto it = std::find(mesh->positions.begin(), mesh->positions.end(), vertex.pos);
                if (it != mesh->positions.end())
                {
                    mesh->indices.push_back(static_cast<u32>(std::distance(mesh->positions.begin(), it)));
                }
                else
                {
                    mesh->positions.push_back(vertex.pos);
                    mesh->normals.push_back(vertex.normal);
                    // TODO: Add texture coordinates and color
                    // mesh->texCoords.push_back(vertex.texCoord);
                    // mesh->colors.push_back(vertex.color);
                    mesh->indices.push_back(static_cast<u32>(mesh->positions.size() - 1));
                }
            }
        }
    }

    // Write MeshData into a new file
    std::ofstream f("OBJLoader.txt");
    if (!f.is_open())
    {
        std::cout << "Failed to open output.txt file\n";
        return nullptr;
    }

    f << "Vertices: " << mesh->positions.size() << "\n";
    for (const auto& v : mesh->positions)
    {
        f << "v " << v.x << " " << v.y << " " << v.z << "\n";
    }

    f << "Normals: " << mesh->normals.size() << "\n";
    for (const auto& v : mesh->normals)
    {
        f << "vn " << v.x << " " << v.y << " " << v.z << "\n";
    }

    f << "Faces: " << mesh->indices.size() << "\n";
    u32 index = 0;
    for (const i32 i : mesh->indices)
    {
        f << "index " << index++ << ": " << i << "\n";
    }

    m_meshes.push_back(mesh);

    return mesh;
}