#ifndef _JOJ_MATERIAL_H
#define _JOJ_MATERIAL_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "core/defines.h"

#if JPLATFORM_WINDOWS

#include "math/jvector4.h"

namespace joj
{
	struct JAPI Material
	{
		Material();

		JVector4 ambient;
		JVector4 diffuse;
		JVector4 specular; // w = SpecPower
		JVector4 reflect;
	};
}

#endif // JPLATFORM_WINDOWS

#endif // _JOJ_MATERIAL_H