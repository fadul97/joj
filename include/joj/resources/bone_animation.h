#ifndef _JOJ_BONE_ANIMATION_H
#define _JOJ_BONE_ANIMATION_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "core/defines.h"

#include "math/jmath.h"
#include <vector>
#include "key_frame.h"

namespace joj
{
	struct BoneAnimation
	{
		f32 get_start_time() const;
		f32 get_end_time() const;

		void interpolate(f32 t, JFloat4x4& M) const;

		std::vector<KeyFrame> keyframes;
	};
}

#endif // _JOJ_BONE_ANIMATION_H