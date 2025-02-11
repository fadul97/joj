#ifndef _JOJ_SUBMESH_H
#define _JOJ_SUBMESH_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "core/defines.h"

#include <string>

namespace joj
{
	struct JAPI SubMesh
	{
		SubMesh();
		SubMesh(std::string name);
		~SubMesh();

		std::string name; // as ID
		i32 id;
		u32 vertex_start;
		u32 vertex_count;
		u32 face_start;
		u32 face_count;
	};
}

#endif // _JOJ_SUBMESH_H