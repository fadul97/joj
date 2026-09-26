#include "lib.hpp"

// STD Includes
#include <cstdlib>
#include <stdio.h>

namespace lft {

void abort()
{
    printf("Aborting program...\n");
    std::abort();
}

} // namespace lft
