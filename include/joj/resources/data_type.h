#ifndef _JOJ_DATA_TYPE_H
#define _JOJ_DATA_TYPE_H

#include "joj/core/defines.h"

namespace joj
{
    enum class DataType
    {
        VEC2,
        VEC3,
        VEC4,
        SCALAR,
        MAT2,
        MAT3,
        MAT4,
        UNKNOWN
    };

    JOJ_API const char* data_type_to_string(const DataType type);
}

#endif // _JOJ_DATA_TYPE_H