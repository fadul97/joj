#ifndef _JOJ_BUFFER_VIEW_TARGET_H
#define _JOJ_BUFFER_VIEW_TARGET_H

#include "joj/core/defines.h"

namespace joj
{
    enum class BufferViewTarget
    {
        ARRAY_BUFFER,
        ELEMENT_ARRAY_BUFFER,
        ANY,
        UNKNOWN
    };

    JOJ_API const char* buffer_view_target_to_string(const BufferViewTarget target);
}

#endif // _JOJ_BUFFER_VIEW_TARGET_H