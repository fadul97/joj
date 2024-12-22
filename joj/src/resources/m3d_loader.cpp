#include "resources/m3d_loader.h"

#include <fstream>
#include "logger.h"

namespace joj
{
    void read_materials(std::ifstream& fin, u32 num_materials, std::vector<M3DMaterial>& mats);
    void read_submesh_table(std::ifstream& fin, u32 num_submeshes, std::vector<SubMesh>& submeshes);
    void read_vertices(std::ifstream& fin, u32 num_vertices, std::vector<Vertex::PosNormalTexTan>& vertices);
    void read_triangles(std::ifstream& fin, u32 num_triangles, std::vector<u16>& indices);
}


joj::ErrorCode joj::M3DLoader::load_M3D_static(const std::string& filename,
    std::vector<Vertex::PosNormalTexTan>& vertices,
    std::vector<u16>& indices,
    std::vector<SubMesh>& subsets,
    std::vector<M3DMaterial>& mats)
{
    std::ifstream fin(filename);
    if (!fin.is_open())
    {
        JERROR(ErrorCode::ERR_FILE_OPENED_FAILED,
            "Failed to open file '%s'.", filename.c_str());
        return ErrorCode::ERR_FILE_OPENED_FAILED;
    }

    u32 num_materials = 0;
    u32 num_vertices = 0;
    u32 num_triangles = 0;
    u32 num_bones = 0;
    u32 num_animation_clips = 0;

    std::string ignore;
    fin >> ignore; // File header text
    fin >> ignore >> num_materials;
    fin >> ignore >> num_vertices;
    fin >> ignore >> num_triangles;
    fin >> ignore >> num_bones;
    fin >> ignore >> num_animation_clips;

    read_materials(fin, num_materials, mats);
    read_submesh_table(fin, num_materials, subsets);
    read_vertices(fin, num_vertices, vertices);
    read_triangles(fin, num_triangles, indices);

    return ErrorCode::OK;
}

void joj::read_materials(std::ifstream& fin, u32 num_materials, std::vector<M3DMaterial>& mats)
{
    std::string ignore;
    mats.resize(num_materials);

    std::string diffuse_map_name;
    std::string normal_map_name;

    fin >> ignore; // Materials header text
    for (u32 i = 0; i < num_materials; ++i)
    {
        fin >> ignore >> mats[i].mat.ambient.x >> mats[i].mat.ambient.y >> mats[i].mat.ambient.z;
        fin >> ignore >> mats[i].mat.diffuse.x >> mats[i].mat.diffuse.y >> mats[i].mat.diffuse.z;
        fin >> ignore >> mats[i].mat.specular.x >> mats[i].mat.specular.y >> mats[i].mat.specular.z;
        fin >> ignore >> mats[i].mat.specular.w;
        fin >> ignore >> mats[i].mat.reflect.x >> mats[i].mat.reflect.y >> mats[i].mat.reflect.z;
        fin >> ignore >> mats[i].alpha_clip;
        fin >> ignore >> mats[i].effect_type_name;
        fin >> ignore >> diffuse_map_name;
        fin >> ignore >> normal_map_name;

        mats[i].diffuse_map_name.resize(diffuse_map_name.size(), ' ');
        mats[i].normal_map_name.resize(normal_map_name.size(), ' ');
        std::copy(diffuse_map_name.begin(), diffuse_map_name.end(), mats[i].diffuse_map_name.begin());
        std::copy(normal_map_name.begin(), normal_map_name.end(), mats[i].normal_map_name.begin());
    }
}

void joj::read_submesh_table(std::ifstream& fin, u32 num_submeshes, std::vector<SubMesh>& submeshes)
{
    std::string ignore;
    submeshes.resize(num_submeshes);

    fin >> ignore; // Submesh header text
    for (u32 i = 0; i < num_submeshes; ++i)
    {
        fin >> ignore >> submeshes[i].id;
        fin >> ignore >> submeshes[i].vertex_start;
        fin >> ignore >> submeshes[i].vertex_count;
        fin >> ignore >> submeshes[i].face_start;
        fin >> ignore >> submeshes[i].face_count;
    }
}

void joj::read_vertices(std::ifstream& fin, u32 num_vertices, std::vector<Vertex::PosNormalTexTan>& vertices)
{
    std::string ignore;
    vertices.resize(num_vertices);

    fin >> ignore; // Vertices header text
    for (u32 i = 0; i < num_vertices; ++i)
    {
        fin >> ignore >> vertices[i].pos.x >> vertices[i].pos.y >> vertices[i].pos.z;
        fin >> ignore >> vertices[i].tangentU.x >> vertices[i].tangentU.y >> vertices[i].tangentU.z >> vertices[i].tangentU.w;
        fin >> ignore >> vertices[i].normal.x >> vertices[i].normal.y >> vertices[i].normal.z;
        fin >> ignore >> vertices[i].tex.x >> vertices[i].tex.y;
    }
}

void joj::read_triangles(std::ifstream& fin, u32 num_triangles, std::vector<u16>& indices)
{
    std::string ignore;
    indices.resize(num_triangles * 3);

    fin >> ignore; // triangles header text
    for (u32 i = 0; i < num_triangles; ++i)
    {
        fin >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
    }
}