#include "joj/renderer/renderer.h"


#include <stdio.h>

void joj::renderer_print()
{
	printf("Hello from %s jojRenderer!\n",
#ifdef JOJ_BUILDING_DLL
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