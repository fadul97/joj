#ifndef _JOJ_BUFFER_H
#define _JOJ_BUFFER_H

#include "joj/core/defines.h"

#include <vector>
#include "buffer_type.h"
#include <string>

namespace joj
{
    struct JOJ_API Buffer
    {
        Buffer();
        Buffer(const char* filename, const BufferType type, const std::vector<u8>& data);
        ~Buffer();

        std::string filename;
        std::vector<u8> data;
        BufferType type;
        u32 size;
    };
}

#endif // _JOJ_BUFFER_H