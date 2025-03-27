#ifndef _JOJ_BUFFER_TYPE_H
#define _JOJ_BUFFER_TYPE_H

#include "joj/core/defines.h"

namespace joj
{
    enum class BufferType
    {
        BYTE,
        POSITION,
        NORMAL,
        INDEX,
        ANIMATION,
        TRANSLATION,
        ROTATION,
        SCALE,
        UNKNOWN
    };

    JOJ_API const char* buffer_type_to_string(const BufferType type);
}

#endif // _JOJ_BUFFER_TYPE_H