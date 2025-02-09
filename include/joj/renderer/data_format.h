#ifndef _JOJ_DATA_FORMAT_H
#define _JOJ_DATA_FORMAT_H

namespace joj
{
    enum class DataFormat
    {
        UNDEFINED,
        R32G32B32A32_FLOAT,
        R32G32B32_FLOAT,
        R32G32_FLOAT,
        R32_UINT,
        R16_UINT,
        R32G32B32A32_UINT,
        R8G8B8A8_UINT,
    };
}

#endif // _JOJ_DATA_FORMAT_H