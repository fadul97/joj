#ifndef _JOJ_RASTERIZER_STATE_H
#define _JOJ_RASTERIZER_STATE_H

namespace joj
{
    /**
     * @brief RasterizerState is an enum class that holds the rasterizer state.
     * It is used to set the rasterizer state of the pipeline.
     * 
     * The rasterizer state can be set to Wireframe or Solid.
     */
    enum class RasterizerState
    {
        Wireframe,
        Solid
    };
}

#endif // _JOJ_RASTERIZER_STATE_H