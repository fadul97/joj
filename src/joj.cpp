#include "joj.h"

#include <stdio.h>

void joj_print()
{
	printf("Hello from %s JOJ Library!\n",
#ifdef JOJ_BUILDING_DLL
		"shared"
#else
		"static"
#endif
	);
}