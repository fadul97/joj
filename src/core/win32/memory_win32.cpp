#include "core/memory.h"

#if JOJ_PLATFORM_WINDOWS

#include <string.h>

void* joj::Memory::zero(void* block, u64 size)
{
    return memset(block, 0, size);
}

#endif // JOJ_PLATFORM_WINDOWS
