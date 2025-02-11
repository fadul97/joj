#include "platform/window.h"

#include <stdio.h>

void joj::window_print()
{
	printf("Hello from %s jojPlatform!\n",
#ifdef BUILDING_JOJ_DLL
		"shared"
#else
		"static"
#endif
	);
}