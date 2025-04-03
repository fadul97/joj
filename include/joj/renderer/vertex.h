#ifndef _JOJ_VERTEX_H
#define _JOJ_VERTEX_H

#include "joj/core/defines.h"

#include "joj/core/math/vector3.h"
#include "joj/core/math/vector4.h"

#if JOJ_PLATFORM_WINDOWS
#include <DirectXMath.h>
#elif JOJ_PLATFORM_LINUX
#include "joj/core/math/vector2.h"
#endif

namespace joj
{
#if JOJ_PLATFORM_WINDOWS
    using Vector2 = DirectX::XMFLOAT2;
#endif

    namespace Vertex
    {
		struct ColorTanPosNormalTex
		{
			Vector4 color;
			Vector4 tangentU;
			Vector3 pos;
			Vector3 normal;
			Vector2 tex;
		};

		struct PosColor
		{
            Vector3 pos;
            Vector4 color;
		};

		struct PosColorNormal
		{
			Vector3 pos;
			Vector4 color;
			Vector3 normal;
		};

		struct PosNormal
		{
			Vector3 pos;
			Vector3 normal;
		};

        // TODO: Find a better name for this struct
        struct RectUIType
		{
            Vector3 pos;
            Vector4 color;
		};
		
		struct PosColorUVRect
		{
			Vector3 pos;
			Vector4 color;
			Vector4 uv_rect;
		};

        struct PosColorNormalOP
        {
            Vector3 pos;
            Vector4 color;
            Vector3 normal;

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
			Vector3 pos;
			Vector3 normal;
			Vector2 tex;
		};

		struct PosNormalTexTan
		{
			Vector3 pos;
			Vector3 normal;
			Vector2 tex;
			Vector4 tangentU;

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
			Vector3 pos;
			Vector3 normal;
			Vector2 tex;
			Vector4 tangentU;
			Vector3 weights;
			// FIXME: bone_indices should not be a u32[4]
			u32 bone_indices[4];
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