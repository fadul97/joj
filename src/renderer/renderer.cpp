#include "renderer/renderer.h"


#include <stdio.h>

void joj::renderer_print()
{
	printf("Hello from %s jojRenderer!\n",
#ifdef BUILDING_JOJ_DLL
		"shared"
#else
		"static"
#endif
	);
}

joj::IRenderer::IRenderer()
{
}

joj::IRenderer::~IRenderer()
{
}