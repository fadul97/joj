#ifndef _JOJ_BUFFER_H
#define _JOJ_BUFFER_H

#include "joj/core/defines.h"

#include <vector>
#include "buffer_type.h"

namespace joj
{
    struct JOJ_API Buffer
    {
        Buffer();
        ~Buffer();

        BufferType type;
        std::vector<u8> data;
    };
}

#endif // _JOJ_BUFFER_H