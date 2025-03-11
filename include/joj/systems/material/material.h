#ifndef _JOJ_MATERIAL_H
#define _JOJ_MATERIAL_H

#include "core/defines.h"

#if JOJ_PLATFORM_WINDOWS

#include "math/jvector4.h"

namespace joj
{
	struct JOJ_API Material
	{
		Material();

		JVector4 ambient;
		JVector4 diffuse;
		JVector4 specular; // w = SpecPower
		JVector4 reflect;
	};
}

#endif // JOJ_PLATFORM_WINDOWS

#endif // _JOJ_MATERIAL_H