#ifndef _JOJ_ANIMATION_CLIP_H
#define _JOJ_ANIMATION_CLIP_H

#include "core/defines.h"

#include "math/jmath.h"
#include <vector>
#include "bone_animation.h"

namespace joj
{
	struct AnimationClip
	{
		f32 get_clip_start_time() const;
		f32 get_clip_end_time() const;

		void interpolate(f32 t, std::vector<JFloat4x4>& bone_transforms) const;

		std::vector<BoneAnimation> bone_animations;
	};
}

#endif // _JOJ_ANIMATION_CLIP_H