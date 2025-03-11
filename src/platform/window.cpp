#include "joj/platform/window.h"

#include <stdio.h>

void joj::window_print()
{
	printf("Hello from %s jojPlatform!\n",
#ifdef JOJ_BUILDING_DLL
		"shared"
#else
		"static"
#endif
	);
}