#ifndef _JOJ_SKINNED_DATA_H
#define _JOJ_SKINNED_DATA_H

#include "core/defines.h"

#include <vector>
#include "math/jmath.h"
#include <string>
#include <map>
#include "animation_clip.h"

namespace joj
{
    class JAPI SkinnedData
    {
    public:
        SkinnedData();
        ~SkinnedData();

        void set(std::vector<int>& bone_hierarchy,
            std::vector<JFloat4x4>& bone_offsets,
            std::map<std::string, AnimationClip>& animations);

        u32 get_bone_count() const;

        f32 get_clip_start_time(const std::string& clip_name) const;
        f32 get_clip_end_time(const std::string& clip_name) const;

        void get_final_transforms(const std::string& clip_name, f32 time_pos,
            std::vector<JFloat4x4>& final_transforms) const;

    private:
        // Gives parentIndex of ith bone.
        std::vector<i32> m_bone_hierarchy;

        std::vector<JFloat4x4> m_bone_offsets;

        std::map<std::string, AnimationClip> m_animations;
    };
}

#endif // _JOJ_SKINNED_DATA_H