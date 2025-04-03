#ifndef _JOJ_LIGHT_H
#define _JOJ_LIGHT_H

#include "joj/core/defines.h"

#include "joj/math/jmath.h"
#include "joj/core/math/vector3.h"
#include "joj/core/math/vector4.h"

namespace joj
{
	struct JOJ_API DirectionalLight
	{
		DirectionalLight();

		Vector4 ambient;
		Vector4 diffuse;
		Vector4 specular;
		Vector3 direction;
		f32 pad; // Padding
	};

	struct JOJ_API PointLight
	{
		PointLight();

		Vector4 ambient;
		Vector4 diffuse;
		Vector4 specular;

		// HLSL: Packed into 4D vector: (Position, Range)
		Vector3 position;
		f32 range;

		// HLSL: Packed into 4D vector: (A0, A1, A2, pad)
		Vector3 att;
		f32 pad; // Padding
	};

	struct JOJ_API SpotLight
	{
		SpotLight();

		Vector4 ambient;
		Vector4 diffuse;
		Vector4 specular;

		// HLSL: Packed into 4D vector: (Position, Range)
		Vector3 position;
		f32 range;

		// HLSL: Packed into 4D vector: (direction, Spot)
		Vector3 direction;
		f32 spot;

		// HLSL: Packed into 4D vector: (att, pad)
		Vector3 att;
		f32 pad; // Padding
	};
}

#endif // _JOJ_LIGHT_H