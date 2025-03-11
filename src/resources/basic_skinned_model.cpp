#include "joj/resources/basic_skinned_model.h"

joj::BasicSkinnedModel::BasicSkinnedModel()
    : m_submesh_count{ 0 }, m_mesh{ nullptr }
{
}

joj::BasicSkinnedModel::~BasicSkinnedModel()
{
}
/*
BasicSkinnedModel* model;
        f32 time_pos;
        std::string clip_name;
        JFloat4x4 world;
        std::vector<JFloat4x4> final_transforms;
*/
joj::SkinnedModelInstance::SkinnedModelInstance()
    : model{ nullptr }, time_pos{ 0.0f },
    clip_name{ "" }, world{ float4x4_identity() }
{
}

joj::SkinnedModelInstance::~SkinnedModelInstance()
{
    // TODO: Delete model -> Not SkinnedModelInstance concern
}

void joj::SkinnedModelInstance::update(f32 dt)
{
    time_pos += dt;
    model->get_skinned_data().get_final_transforms(clip_name, time_pos, final_transforms);

    // Loop animation
    if (time_pos > model->get_skinned_data().get_clip_end_time(clip_name))
        time_pos = 0.0f;
}