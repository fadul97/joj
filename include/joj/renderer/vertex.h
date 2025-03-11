#ifndef _JOJ_VERTEX_H
#define _JOJ_VERTEX_H

#include "core/defines.h"

#include "math/jmath.h"
#include <wtypes.h>
#include "math/jvector3.h"
#include "math/jvector4.h"

namespace joj
{
    namespace Vertex
    {
		struct PosColor
		{
            JFloat3 pos;
            JFloat4 color;
		};
		
		struct PosColorUVRect
		{
			JFloat3 pos;
			JFloat4 color;
			JFloat4 uv_rect;
		};

        struct PosColorNormal
        {
            JVector3 pos;
            JVector4 color;
            JVector3 normal;

            bool operator==(const PosColorNormal& other) const
            {
                return pos.x == other.pos.x &&
                    pos.y == other.pos.y &&
                    pos.z == other.pos.z &&
                    color.x == other.color.x &&
                    color.y == other.color.y &&
                    color.z == other.color.z &&
                    color.w == other.color.w &&
                    normal.x == other.normal.x &&
                    normal.y == other.normal.y &&
                    normal.z == other.normal.z;
            }
        };

		struct PosNormalTex
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

			bool operator==(const PosNormalTexTan& other) const
			{
				return pos.x == other.pos.x &&
					pos.y == other.pos.y &&
					pos.z == other.pos.z &&
					normal.x == other.normal.x &&
					normal.y == other.normal.y &&
					normal.z == other.normal.z &&
					tex.x == other.tex.x &&
					tex.y == other.tex.y &&
					tangentU.x == other.tangentU.x &&
					tangentU.y == other.tangentU.y &&
					tangentU.z == other.tangentU.z &&
					tangentU.w == other.tangentU.w;
			}
		};

		inline b8 compare_vertex_PosNormalTexTan(const PosNormalTexTan& lhs, const PosNormalTexTan& rhs)
		{
			return lhs.pos.x == rhs.pos.x &&
				lhs.pos.y == rhs.pos.y &&
				lhs.pos.z == rhs.pos.z &&
				lhs.normal.x == rhs.normal.x &&
				lhs.normal.y == rhs.normal.y &&
				lhs.normal.z == rhs.normal.z &&
				lhs.tex.x == rhs.tex.x &&
				lhs.tex.y == rhs.tex.y &&
				lhs.tangentU.x == rhs.tangentU.x &&
				lhs.tangentU.y == rhs.tangentU.y &&
				lhs.tangentU.z == rhs.tangentU.z &&
				lhs.tangentU.w == rhs.tangentU.w;
		}

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