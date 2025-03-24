#ifndef _JOJ_RENDERER_H
#define _JOJ_RENDERER_H

#include "joj/core/defines.h"

#include "joj/core/error_code.h"
#include "rasterizer_state.h"
#include "primitive_topology.h"
#include "joj/core/color.h"

namespace joj
{
    /**
     * @brief GraphicsDevice is a struct that holds the graphics device.
     * It is API dependent, so each API will have its own implementation.
     *  
     * Forward declaration to avoid increasing compile time.
     */
    struct GraphicsDevice;

    /**
     * @brief CommandList is a struct that holds the command list.
     * It is API dependent, so each API will have its own implementation.
     * 
     * Forward declaration to avoid increasing compile time.
     * 
     */
    struct CommandList;

    /**
     * @brief WindowData is a struct that contains the necessary data that a game
     * window needs to store.
     * It will be defined by the platform, in the corresponding header/source file.
     * 
     * Forward declaration to avoid increasing compile time.
     */
    struct WindowData;

    /**
     * @brief IRenderer is an interface that represents a renderer.
     * It is API dependent, so each API will have its own implementation.
     */
    class JOJ_API IRenderer
    {
    public:
        /**
         * @brief Construct a new IRenderer object.
         */
        IRenderer();

        /**
         * @brief Destroy the IRenderer object.
         * It will call the shutdown and destroy_context methods. If the user just wants to
         * reinitialize the renderer, it is recommended to call these methods separately.
         */
        virtual ~IRenderer();

        /**
         * @brief Destroy the context.
         * It will release the resources that were created in the create_context method.
         * 
         * @note This method is called in the destructor of the IRenderer class.
         * 
         * @note If the user just wants to reinitialize the renderer,
         * it is recommended to call this method separately.
         */
        virtual void destroy_context() = 0;

        /**
         * @brief Initialize the renderer.
         * 
         * @param window The window data.
         * @return ErrorCode The error code that will be returned if the renderer
         * initialization fails. If the renderer initialization is successful, it will return
         * ErrorCode::OK.
         */
        virtual ErrorCode initialize(WindowData* window) = 0;

        /**
         * @brief Shutdown the renderer.
         * It will release the resources that were created in the initialize method.
         * 
         * @note This method is called in the destructor of the IRenderer class.
         * 
         * @note If the user just wants to reinitialize the renderer,
         * it is recommended to call this method separately.
         */
        virtual void shutdown() = 0;

        /**
         * @brief Begin the frame.
         */
        virtual void begin_frame() = 0;

        /**
         * @brief End the frame.
         */
        virtual void end_frame() = 0;

        /**
         * @brief Draw the primitive.
         * 
         * @param vertex_count The number of vertices to draw.
         * @param start_vertex_location The location of the first vertex to draw.
         */
        virtual void draw(const u32 vertex_count, const u32 start_vertex_location) = 0;

        /**
         * @brief Draw the indexed primitive.
         * 
         * @param index_count The number of indices to draw.
         * @param start_index_location The location of the first index to draw.
         * @param base_vertex_location The location of the first vertex to draw.
         */
        virtual void draw_indexed(const u32 index_count, const u32 start_index_location,
            const i32 base_vertex_location) = 0;

        /**
         * @brief Draw a rect with a given color.
         * It will create all the resources needed to draw the rect
         * and then draw it.
         * It is a Immediate Mode GUI method.
         * 
         * @param x The x position of the rect on the screen.
         * @param y The y position of the rect on the screen.
         * @param width The width of the rect.
         * @param height The height of the rect.
         * @param color The color of the rect.
         */
        virtual void draw_rect(const i32 x, const i32 y, const i32 width,
            const i32 height, const Color* color) = 0;

        /**
         * @brief Set the rasterizer state object.
         * 
         * @param state The rasterizer state.
         */
        virtual void set_rasterizer_state(const RasterizerState state) = 0;

        /**
         * @brief Set the primitive topology object.
         * 
         * @param topology The primitive topology.
         */
        virtual void set_primitive_topology(const PrimitiveTopology topology) = 0;

        /**
         * @brief Get the device object
         * 
         * @return GraphicsDevice* The device object.
         */
        virtual GraphicsDevice* get_device() = 0;

        /**
         * @brief Get the command list object
         * 
         * @return CommandList* The command list object.
         */
        virtual CommandList* get_cmd_list() = 0;

    protected:
        /**
         * @brief Create the context.
         * 
         * @note This method is called in the initialize method.
         * 
         * @return ErrorCode The error code that will be returned if the context
         * creation fails. If the context creation is successful, it will return
         * ErrorCode::OK.
         */
        virtual ErrorCode create_context() = 0;
    };
}

#endif // _JOJ_RENDERER_H