#include "joj.h"

#include <stdio.h>

void joj_print()
{
	printf("Hello from %s JOJ Library!\n",
#ifdef BUILDING_JOJ_DLL
		"shared"
#else
		"static"
#endif
	);
}