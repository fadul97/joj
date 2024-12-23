#ifndef _JOJ_VERTEX_H
#define _JOJ_VERTEX_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "math/jmath.h"
#include <wtypes.h>

namespace joj
{
    namespace Vertex
    {
		struct PorNormalTex
		{
			JFloat3 pos;
			JFloat3 normal;
			JFloat2 tex;
		};

		struct PosNormalTexTan
		{
			JFloat3 pos;
			JFloat3 normal;
			JFloat2 tex;
			JFloat4 tangentU;
		};

		struct PosNormalTexTanSkinned
		{
			JFloat3 pos;
			JFloat3 normal;
			JFloat2 tex;
			JFloat4 tangentU;
			JFloat3 weights;
			BYTE bone_indices[4];
		};
    }

	enum class VertexFormat
	{
		PosNormalTex,
		PosNormalTexTan,
		PosNormalTexTanSkinned
	};
}

#endif // _JOJ_VERTEX_H