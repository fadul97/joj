#include "resources/animation_clip.h"

f32 joj::AnimationClip::get_clip_start_time() const
{
	// Find smallest start time over all bones in this clip.
	f32 t = JINFINITY;
	for (u32 i = 0; i < bone_animations.size(); ++i)
	{
		t = Min(t, bone_animations[i].get_start_time());
	}

	return t;
}

f32 joj::AnimationClip::get_clip_end_time() const
{
	// Find largest end time over all bones in this clip.
	float t = 0.0f;
	for (u32 i = 0; i < bone_animations.size(); ++i)
	{
		t = Max(t, bone_animations[i].get_end_time());
	}

	return t;
}

void joj::AnimationClip::interpolate(f32 t, std::vector<JFloat4x4>& bone_transforms) const
{
	for (u32 i = 0; i < bone_animations.size(); ++i)
	{
		bone_animations[i].interpolate(t, bone_transforms[i]);
	}
}