#include "resources/m3d_loader.h"

#include <fstream>
#include "core/logger.h"

namespace joj
{
    void read_materials(std::ifstream& fin, u32 num_materials, std::vector<M3DMaterial>& mats);
    void read_submesh_table(std::ifstream& fin, u32 num_submeshes, std::vector<SubMesh>& submeshes);
    void read_vertices(std::ifstream& fin, u32 num_vertices, std::vector<Vertex::PosNormalTexTan>& vertices);
    void read_triangles(std::ifstream& fin, u32 num_triangles, std::vector<u16>& indices);

    void read_skinned_vertices(std::ifstream& fin, u32 num_vertices, std::vector<Vertex::PosNormalTexTanSkinned>& vertices);
    void read_bone_offsets(std::ifstream& fin, u32 num_bones, std::vector<JFloat4x4>& bone_offsets);
    void read_bone_hierarchy(std::ifstream& fin, u32 num_bones, std::vector<i32>& bone_index_to_parent_index);
    void read_animation_clips(std::ifstream& fin, u32 num_bones, u32 num_animation_clips, std::map<std::string, AnimationClip>& animations);
    void read_bone_keyframes(std::ifstream& fin, u32 num_bones, BoneAnimation& bone_animation);
}


joj::ErrorCode joj::M3DLoader::load_M3D_static(const std::string& filename,
    std::vector<Vertex::PosNormalTexTan>& vertices,
    std::vector<u16>& indices,
    std::vector<SubMesh>& submeshes,
    std::vector<M3DMaterial>& mats)
{
    std::ifstream fin(filename);
    if (!fin.is_open())
    {
        JOJ_ERROR(ErrorCode::ERR_FILE_OPENED_FAILED,
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
    read_submesh_table(fin, num_materials, submeshes);
    read_vertices(fin, num_vertices, vertices);
    read_triangles(fin, num_triangles, indices);

    return ErrorCode::OK;
}

joj::ErrorCode joj::M3DLoader::load_M3D_skinned(const std::string& filename,
    std::vector<Vertex::PosNormalTexTanSkinned>& vertices,
    std::vector<u16>& indices,
    std::vector<SubMesh>& submeshes,
    std::vector<M3DMaterial>& mats,
    SkinnedData& skin_info)
{
    std::ifstream fin(filename);
    if (!fin.is_open())
    {
        JOJ_ERROR(ErrorCode::ERR_FILE_OPENED_FAILED,
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

    std::vector<JFloat4x4> bone_offsets;
    std::vector<i32> bone_index_to_parent_index;
    std::map<std::string, AnimationClip> animations;

    read_materials(fin, num_materials, mats);
    read_submesh_table(fin, num_materials, submeshes);
    read_skinned_vertices(fin, num_vertices, vertices);
    read_triangles(fin, num_triangles, indices);
    read_bone_offsets(fin, num_bones, bone_offsets);
    read_bone_hierarchy(fin, num_bones, bone_index_to_parent_index);
    read_animation_clips(fin, num_bones, num_animation_clips, animations);

    skin_info.set(bone_index_to_parent_index, bone_offsets, animations);

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

void joj::read_skinned_vertices(std::ifstream& fin, u32 num_vertices, std::vector<Vertex::PosNormalTexTanSkinned>& vertices)
{
    std::string ignore;
    vertices.resize(num_vertices);

    fin >> ignore; // Vertices header text
    i32 bone_indices[4];
    f32 weights[4];

    for (u32 i = 0; i < num_vertices; ++i)
    {
        fin >> ignore >> vertices[i].pos.x >> vertices[i].pos.y >> vertices[i].pos.z;
        fin >> ignore >> vertices[i].tangentU.x >> vertices[i].tangentU.y >> vertices[i].tangentU.z >> vertices[i].tangentU.w;
        fin >> ignore >> vertices[i].normal.x >> vertices[i].normal.y >> vertices[i].normal.z;
        fin >> ignore >> vertices[i].tex.x >> vertices[i].tex.y;
        fin >> ignore >> weights[0] >> weights[1] >> weights[2] >> weights[3];
        fin >> ignore >> bone_indices[0] >> bone_indices[1] >> bone_indices[2] >> bone_indices[3];

        vertices[i].weights.x = weights[0];
        vertices[i].weights.y = weights[1];
        vertices[i].weights.z = weights[2];

        vertices[i].bone_indices[0] = (BYTE)bone_indices[0];
        vertices[i].bone_indices[1] = (BYTE)bone_indices[1];
        vertices[i].bone_indices[2] = (BYTE)bone_indices[2];
        vertices[i].bone_indices[3] = (BYTE)bone_indices[3];
    }
}

void joj::read_bone_offsets(std::ifstream& fin, u32 num_bones, std::vector<JFloat4x4>& bone_offsets)
{
    std::string ignore;
    bone_offsets.resize(num_bones);

    fin >> ignore; // BoneOffsets header text
    for (u32 i = 0; i < num_bones; ++i)
    {
        fin >> ignore >>
            bone_offsets[i](0, 0) >> bone_offsets[i](0, 1) >> bone_offsets[i](0, 2) >> bone_offsets[i](0, 3) >>
            bone_offsets[i](1, 0) >> bone_offsets[i](1, 1) >> bone_offsets[i](1, 2) >> bone_offsets[i](1, 3) >>
            bone_offsets[i](2, 0) >> bone_offsets[i](2, 1) >> bone_offsets[i](2, 2) >> bone_offsets[i](2, 3) >>
            bone_offsets[i](3, 0) >> bone_offsets[i](3, 1) >> bone_offsets[i](3, 2) >> bone_offsets[i](3, 3);
    }
}

void joj::read_bone_hierarchy(std::ifstream& fin, u32 num_bones, std::vector<i32>& bone_index_to_parent_index)
{
    std::string ignore;
    bone_index_to_parent_index.resize(num_bones);

    fin >> ignore; // BoneHierarchy header text
    for (u32 i = 0; i < num_bones; ++i)
    {
        fin >> ignore >> bone_index_to_parent_index[i];
    }
}

void joj::read_animation_clips(std::ifstream& fin, u32 num_bones, u32 num_animation_clips, std::map<std::string, AnimationClip>& animations)
{
    std::string ignore;
    fin >> ignore; // AnimationClips header text

    for (u32 clip_index = 0; clip_index < num_animation_clips; ++clip_index)
    {
        std::string clip_name;
        fin >> ignore >> clip_name;
        fin >> ignore; // {

        AnimationClip clip;
        clip.bone_animations.resize(num_bones);

        for (u32 bone_index = 0; bone_index < num_bones; ++bone_index)
        {
            read_bone_keyframes(fin, num_bones, clip.bone_animations[bone_index]);
        }
        fin >> ignore; // }

        animations[clip_name] = clip;
    }
}
void joj::read_bone_keyframes(std::ifstream& fin, u32 num_bones, BoneAnimation& bone_animation)
{
    std::string ignore;
    u32 num_keyframes = 0;
    fin >> ignore >> ignore >> num_keyframes;
    fin >> ignore; // {

    bone_animation.keyframes.resize(num_keyframes);

    for (u32 i = 0; i < num_keyframes; ++i)
    {
        f32 t = 0.0f;
        JFloat3 p(0.0f, 0.0f, 0.0f);
        JFloat3 s(1.0f, 1.0f, 1.0f);
        JFloat4 q(0.0f, 0.0f, 0.0f, 1.0f);
        fin >> ignore >> t;
        fin >> ignore >> p.x >> p.y >> p.z;
        fin >> ignore >> s.x >> s.y >> s.z;
        fin >> ignore >> q.x >> q.y >> q.z >> q.w;

        bone_animation.keyframes[i].time_pos = t;
        bone_animation.keyframes[i].translation = p;
        bone_animation.keyframes[i].scale = s;
        bone_animation.keyframes[i].rotation_quat = q;
    }

    fin >> ignore; // }
}