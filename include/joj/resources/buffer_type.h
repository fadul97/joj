#ifndef _JOJ_BUFFER_TYPE_H
#define _JOJ_BUFFER_TYPE_H

namespace joj
{
    enum class BufferType
    {
        POSITION,
        NORMAL,
        INDEX,
        ANIMATION,
        TRANSLATION,
        ROTATION,
        SCALE,
        UNKNOWN
    };
}

#endif // _JOJ_BUFFER_TYPE_H