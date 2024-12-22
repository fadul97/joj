#ifndef _JOJ_DATA_FORMAT_H
#define _JOJ_DATA_FORMAT_H

/*
DXGI_FORMAT_R32G32B32_FLOAT
DXGI_FORMAT_R32G32_FLOAT
DXGI_FORMAT_R32G32B32_FLOAT
*/

namespace joj
{
    enum class DataFormat
    {
        R32G32B32_FLOAT,
        R32G32_FLOAT,
        R32_UINT,
        R16_UINT
    };
}

#endif // _JOJ_DATA_FORMAT_H