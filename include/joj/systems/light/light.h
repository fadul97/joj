#ifndef _JOJ_LIGHT_H
#define _JOJ_LIGHT_H

#include "core/defines.h"

#include "math/jmath.h"

namespace joj
{
	struct JOJ_API DirectionalLight
	{
		DirectionalLight();

		JFloat4 ambient;
		JFloat4 diffuse;
		JFloat4 specular;
		JFloat3 direction;
		f32 pad; // Padding
	};

	struct JOJ_API PointLight
	{
		PointLight();

		JFloat4 ambient;
		JFloat4 diffuse;
		JFloat4 specular;

		// HLSL: Packed into 4D vector: (Position, Range)
		JFloat3 position;
		f32 range;

		// HLSL: Packed into 4D vector: (A0, A1, A2, pad)
		JFloat3 att;
		f32 pad; // Padding
	};

	struct JOJ_API SpotLight
	{
		SpotLight();

		JFloat4 ambient;
		JFloat4 diffuse;
		JFloat4 specular;

		// HLSL: Packed into 4D vector: (Position, Range)
		JFloat3 position;
		f32 range;

		// HLSL: Packed into 4D vector: (direction, Spot)
		JFloat3 direction;
		f32 spot;

		// HLSL: Packed into 4D vector: (att, pad)
		JFloat3 att;
		f32 pad; // Padding
	};
}

#endif // _JOJ_LIGHT_H