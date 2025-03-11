#include "joj/resources/skinned_data.h"

joj::SkinnedData::SkinnedData()
{
}

joj::SkinnedData::~SkinnedData()
{
}

void joj::SkinnedData::set(std::vector<int>& bone_hierarchy,
    std::vector<JFloat4x4>& bone_offsets,
    std::map<std::string, AnimationClip>& animations)
{
    m_bone_hierarchy = bone_hierarchy;
    m_bone_offsets = bone_offsets;
    m_animations = animations;
}

u32 joj::SkinnedData::get_bone_count() const
{
    return static_cast<u32>(m_bone_hierarchy.size());
}

f32 joj::SkinnedData::get_clip_start_time(const std::string& clip_name) const
{
    auto clip = m_animations.find(clip_name);
    return clip->second.get_clip_start_time();
}

f32 joj::SkinnedData::get_clip_end_time(const std::string& clip_name) const
{
    auto clip = m_animations.find(clip_name);
    return clip->second.get_clip_end_time();
}

void joj::SkinnedData::get_final_transforms(const std::string& clip_name,
    f32 time_pos, std::vector<JFloat4x4>& final_transforms) const
{
    u32 num_bones = static_cast<u32>(m_bone_offsets.size());

    std::vector<JFloat4x4> to_parent_transforms(num_bones);

    // Interpolate all the bones of this clip at the given time instance.
    auto clip = m_animations.find(clip_name);
    clip->second.interpolate(time_pos, to_parent_transforms);

    // Traverse the hierarchy and transform all the bones to the root space.
    std::vector<JFloat4x4> to_root_transforms(num_bones);

    // The root bone has index 0.  The root bone has no parent, so its toRootTransform
    // is just its local bone transform.
    to_root_transforms[0] = to_parent_transforms[0];

    // Now find the toRootTransform of the children.
    for (u32 i = 1; i < num_bones; ++i)
    {
        JMatrix4x4 to_parent = DirectX::XMLoadFloat4x4(&to_parent_transforms[i]);

        i32 parent_index = m_bone_hierarchy[i];
        JMatrix4x4 parent_to_root = XMLoadFloat4x4(&to_root_transforms[parent_index]);

        JMatrix4x4 to_root = XMMatrixMultiply(to_parent, parent_to_root);

        XMStoreFloat4x4(&to_root_transforms[i], to_root);
    }

    // Premultiply by the bone offset transform to get the final transform.
    for (u32 i = 0; i < num_bones; ++i)
    {
        JMatrix4x4 offset = DirectX::XMLoadFloat4x4(&m_bone_offsets[i]);
        JMatrix4x4 to_root = DirectX::XMLoadFloat4x4(&to_root_transforms[i]);
        XMStoreFloat4x4(&final_transforms[i], XMMatrixMultiply(offset, to_root));
    }
}