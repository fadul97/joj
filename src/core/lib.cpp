#include "joj/core/lib.hpp"

// STD Includes
#include <cstdlib>
#include <stdio.h>

namespace joj {

void abort()
{
    printf("Aborting program...\n");
    std::abort();
}

} // namespace joj
