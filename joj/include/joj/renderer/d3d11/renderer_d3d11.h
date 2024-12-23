#ifndef _JOJ_D3D11_RENDERER_H
#define _JOJ_D3D11_RENDERER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include "renderer/renderer.h"
#include <d3d11.h>
#include <dxgi1_6.h>

#if JOJ_DEBUG_MODE
#include <dxgidebug.h>
#endif // JOJ_DEBUG_MODE

namespace joj
{
    struct GraphicsDevice
    {
        ID3D11Device* device;
    };

    struct CommandList
    {
        ID3D11DeviceContext* device_context;
    };

    class JAPI D3D11Renderer : public IRenderer
    {
    public:
        D3D11Renderer();
        ~D3D11Renderer();

        ErrorCode create_context() override;
        void destroy_context() override;

        ErrorCode initialize(WindowData window) override;
        void shutdown() override;

        void enable_depth_test() override;
        void disable_depth_test() override;

        void set_rasterizer_state(const RasterizerState state) override;
        void set_primitive_topology(const PrimitiveTopology topology) override;

        void resize(i32 width, i32 height) override;

        GraphicsDevice& get_device() override;
        CommandList& get_cmd_list() override;

        void clear(f32 r = 0.23f, f32 g = 0.23f, f32 b = 0.23f, f32 a = 1.0f) override;
        void swap_buffers() override;

#if JOJ_DEBUG_MODE
        void log_hardware_info();
#endif // JOJ_DEBUG_MODE

    private:
        GraphicsDevice m_graphics_device;
        CommandList m_cmd_list;

        IDXGIFactory6* m_factory;

        b8 m_context_created;

        b8 m_4xmsaa_enabled;
        u32 m_4xmsaa_quality;
        b8 m_vsync;
        u32 m_buffer_count;

        IDXGISwapChain* m_swapchain;                                // Swap chain
        ID3D11RenderTargetView* m_render_target_view;               // Backbuffer render target view
        ID3D11Texture2D* m_depth_stencil_buffer;
        ID3D11DepthStencilState* m_depth_stencil_state;
        ID3D11DepthStencilState* m_depth_disabled_stencil_state;    // Disabled depth stencil
        ID3D11DepthStencilView* m_depth_stencil_view;               // Depth/Stencil view
        D3D11_VIEWPORT m_viewport;                                  // Viewport
        ID3D11BlendState* m_blend_state;                            // Color mix settings
        ID3D11RasterizerState* m_rasterizer_state_solid;            // Solid Rasterizer state
        ID3D11RasterizerState* m_rasterizer_state_wireframe;        // Wireframe Rasterizer state

#if JOJ_DEBUG_MODE
        ID3D11Debug* m_debug;
#endif // JOJ_DEBUG_MODE
    };
}

#endif // JPLATFORM_WINDOWS

#endif // _JOJ_D3D11_RENDERER_H