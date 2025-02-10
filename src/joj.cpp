#include "joj.h"

#include <stdio.h>

void joj_print()
{
	printf("Hello from %s JOJ Library!\n",
#ifdef JAPI
		"shared"
#else
		"static"
#endif
	);
}