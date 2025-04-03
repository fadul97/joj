#ifndef _JOJ_MATERIAL_H
#define _JOJ_MATERIAL_H

#include "joj/core/defines.h"

#include "joj/core/math/vector4.h"

namespace joj
{
	struct JOJ_API Material
	{
		Material();

		Vector4 ambient;
		Vector4 diffuse;
		Vector4 specular; // w = SpecPower
		Vector4 reflect;
	};
}

#endif // _JOJ_MATERIAL_H