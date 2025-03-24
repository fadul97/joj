#ifndef _JOJ_DATA_FORMAT_H
#define _JOJ_DATA_FORMAT_H

namespace joj
{
    /**
     * @brief DataFormat is an enum class that holds the data format.
     * It is used to set the data format of the input layout.
     * 
     * The data format can be set to:
     * - R32G32B32A32_FLOAT, R32G32B32_FLOAT, R32G32_FLOAT, R32_UINT, R16_UINT, R32G32B32A32_UINT, or R8G8B8A8_UINT.
     */
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