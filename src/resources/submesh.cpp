#include "joj/resources/submesh.h"

joj::SubMesh::SubMesh()
	: name{ "" }, id{ -1 },
	vertex_start{ 0 }, vertex_count{ 0 },
	face_start{ 0 }, face_count{ 0 }
{
}

joj::SubMesh::SubMesh(std::string name)
	: name{ name }, id{ -1 },
	vertex_start{ 0 }, vertex_count{ 0 },
	face_start{ 0 }, face_count{ 0 }
{
}

joj::SubMesh::~SubMesh()
{
}