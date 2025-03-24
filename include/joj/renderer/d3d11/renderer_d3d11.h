#ifndef _JOJ_D3D11_RENDERER_H
#define _JOJ_D3D11_RENDERER_H

#include "joj/core/defines.h"

#if JOJ_PLATFORM_WINDOWS

#include "joj/renderer/renderer.h"
#include "graphics_device_d3d11.h"
#include "command_list_d3d11.h"
#include <d3d11.h>
#include <dxgi1_6.h>

#if JOJ_DEBUG_MODE
#include <dxgidebug.h>
#endif // JOJ_DEBUG_MODE

namespace joj
{
    /**
     * @brief D3D11Renderer is a class that represents the DirectX 11 renderer.
     * It inherits from IRenderer.
     */
    class JOJ_API D3D11Renderer : public IRenderer
    {
    public:
        /**
         * @brief Construct a new D3D11Renderer object.
         */
        D3D11Renderer();

        /**
         * @brief Destroy the D3D11Renderer object.
         * 
         * It will call the shutdown and destroy_context methods. If the user just wants to
         * reinitialize the renderer, it is recommended to call these methods separately.
         */
        ~D3D11Renderer() override;

        /**
         * @brief Destroy the context.
         * It will release the resources that were created in the create_context method.
         * 
         * @note This method is called in the destructor of the D3D11Renderer class.
         * 
         * @note If the user just wants to reinitialize the renderer,
         * it is recommended to call this method separately.
         */
        void destroy_context() override;

        /**
         * @brief Initialize the renderer.
         * 
         * @param window The window data.
         * @return ErrorCode The error code that will be returned if the renderer
         * initialization fails. If the renderer initialization is successful, it will return
         * ErrorCode::OK.
         */
        ErrorCode initialize(WindowData* window) override;

        /**
         * @brief Shutdown the renderer.
         * It will release the resources that were created in the initialize method.
         * 
         * @note This method is called in the destructor of the D3D11Renderer class.
         * 
         * @note If the user just wants to reinitialize the renderer,
         * it is recommended to call this method separately.
         */
        void shutdown() override;

        /**
         * @brief Begin the frame.
         */
        void begin_frame() override;

        /**
         * @brief End the frame.
         */
        void end_frame() override;

        /**
         * @brief Draw the primitive.
         * 
         * @param vertex_count The number of vertices to draw.
         * @param start_vertex_location The location of the first vertex to draw.
         */
        void draw(const u32 vertex_count, const u32 start_vertex_location) override;

        /**
         * @brief Draw the indexed primitive.
         * 
         * @param index_count The number of indices to draw.
         * @param start_index_location The location of the first index to draw.
         * @param base_vertex_location The location of the first vertex to draw.
         */
        void draw_indexed(const u32 index_count, const u32 start_index_location,
            const i32 base_vertex_location) override;

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
        void draw_rect(const i32 x, const i32 y, const i32 width,
            const i32 height, const Color* color) override;

        /**
         * @brief Set the rasterizer state object.
         * 
         * @param state The rasterizer state.
         */
        void set_rasterizer_state(const RasterizerState state) override;

        /**
         * @brief Set the primitive topology object.
         * 
         * @param topology The primitive topology.
         */
        void set_primitive_topology(const PrimitiveTopology topology) override;

        /**
         * @brief Get the device object
         * 
         * @return GraphicsDevice* The device object.
         */
        GraphicsDevice* get_device() override;

        /**
         * @brief Get the command list object
         * 
         * @return CommandList* The command list object.
         */
        CommandList* get_cmd_list() override;

#if JOJ_DEBUG_MODE
        /**
         * @brief Log the hardware info.
         */
        void log_hardware_info();
#endif // JOJ_DEBUG_MODE

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
        ErrorCode create_context() override;

    private:
        // ---------------------------------------------------
        // Graphics device initialization.
        // ---------------------------------------------------

        /** @brief The graphics device. */
        GraphicsDevice m_graphics_device;


        /** @brief The command list. */
        CommandList m_cmd_list;

        /** @brief The factory for DirectX 11. */
        IDXGIFactory6* m_factory;

        /** @brief Boolean to check if 4x MSAA is enabled. Default is false. */
        b8 m_4xmsaa_enabled;

        /** @brief The 4x MSAA quality. Default is 0. */
        u32 m_4xmsaa_quality;

        /** @brief The VSync. Default is true. */
        b8 m_vsync;

        /** @brief The buffer count. Default is 2. */
        u32 m_buffer_count;

        // ---------------------------------------------------
        // Graphics Pipeline initialization.
        // ---------------------------------------------------

        /** @brief The swap chain. */
        IDXGISwapChain* m_swapchain;

        /** @brief The backbuffer. */
        ID3D11RenderTargetView* m_render_target_view;

        /** @brief The depth stencil buffer. */
        ID3D11Texture2D* m_depth_stencil_buffer;

        /** @brief The depth stencil state. */
        ID3D11DepthStencilState* m_depth_stencil_state;

        /** @brief The depth stencil state for disabled depth. */
        ID3D11DepthStencilState* m_depth_disabled_stencil_state;

        /** @brief The depth stencil view. */
        ID3D11DepthStencilView* m_depth_stencil_view;

        /** @brief The Viewport. */
        D3D11_VIEWPORT m_viewport;
        
        /** @brief The blend state. */
        ID3D11BlendState* m_blend_state;

        /** @brief The rasterizer state for solid. */
        ID3D11RasterizerState* m_rasterizer_state_solid;

        /** @brief The rasterizer state for wireframe. */
        ID3D11RasterizerState* m_rasterizer_state_wireframe;

#if JOJ_DEBUG_MODE
        /** @brief The debug object. */
        ID3D11Debug* m_debug;
#endif // JOJ_DEBUG_MODE
    };
}

#endif // JOJ_PLATFORM_WINDOWS

#endif // _JOJ_D3D11_RENDERER_H