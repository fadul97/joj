#ifndef _JOJ_VERTEX_H
#define _JOJ_VERTEX_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "math/jmath.h"

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
    }

	enum class VertexFormat
	{
		PosNormalTex,
		PosNormalTexTan
	};
}

#endif // _JOJ_VERTEX_H