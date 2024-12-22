#ifndef _JOJ_MATERIAL_H
#define _JOJ_MATERIAL_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include "math/jmath.h"

namespace joj
{
	struct Material
	{
		Material();

		JFloat4 ambient;
		JFloat4 diffuse;
		JFloat4 specular; // w = SpecPower
		JFloat4 reflect;
	};
}

#endif // JPLATFORM_WINDOWS

#endif // _JOJ_MATERIAL_H