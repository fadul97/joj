#ifndef _JOJ_PRIMITIVE_TOPOLOGY_H
#define _JOJ_PRIMITIVE_TOPOLOGY_H

namespace joj
{
    /**
     * @brief PrimitiveTopology is an enum class that holds the primitive topology.
     * It is used to set the primitive topology of the pipeline.
     * 
     * The primitive topology can be set to:
     * - PointList, LineList, LineStrip, TriangleList, or TriangleStrip.
     */
    enum class PrimitiveTopology
    {
        UNDEFINED,
        POINT_LIST,
        LINE_LIST,
        LINE_STRIP,
        TRIANGLE_LIST,
        TRIANGLE_STRIP,
    };
}

#endif // _JOJ_PRIMITIVE_TOPOLOGY_H