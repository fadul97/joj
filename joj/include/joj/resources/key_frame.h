#ifndef _JOJ_KEY_FRAME_H
#define _JOJ_KEY_FRAME_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "math/jmath.h"

namespace joj
{
	struct KeyFrame
	{
		KeyFrame();
		~KeyFrame();

		f32 time_pos;
		JFloat3 translation;
		JFloat3 scale;
		JFloat4 rotation_quat;
	};
}

#endif // _JOJ_KEY_FRAME_H