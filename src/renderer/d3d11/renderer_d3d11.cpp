#include "joj/renderer/d3d11/renderer_d3d11.h"

#if JOJ_PLATFORM_WINDOWS

#include "joj/core/logger.h"
#include "joj/core/jmacros.h"
#include "joj/platform/win32/window_data_win32.h"
#include "joj/renderer/vertex.h"
#include "joj/renderer/d3d11/vertex_buffer_d3d11.h"
#include "joj/renderer/d3d11/index_buffer_d3d11.h"
#include "joj/renderer/d3d11/shader_d3d11.h"
#include "joj/renderer/shader_library.h"

joj::D3D11Renderer::D3D11Renderer()
{
    // ---------------------------------------------------
    // Graphics device initialization.
    // ---------------------------------------------------
    m_graphics_device.device = nullptr;
    m_cmd_list.device_context = nullptr;
    m_factory = nullptr;

    m_4xmsaa_enabled = false; // No antialising
    m_4xmsaa_quality = 0;     // Default quality
    m_vsync = true;           // VSync enabled
    m_buffer_count = 2;       // Double buffering

    // ---------------------------------------------------
    // Graphics Pipeline initialization.
    // ---------------------------------------------------
    m_swapchain = nullptr;
    m_render_target_view;
    m_depth_stencil_buffer = nullptr;
    m_depth_stencil_state = nullptr;
    m_depth_disabled_stencil_state = nullptr;
    m_depth_stencil_view = nullptr;
    m_viewport = { 0 };
    m_blend_state = nullptr;
    m_rasterizer_state_solid = nullptr;
    m_rasterizer_state_wireframe = nullptr;

#if JOJ_DEBUG_MODE
    m_debug = nullptr;
#endif // JOJ_DEBUG_MODE
}

joj::D3D11Renderer::~D3D11Renderer()
{
    shutdown();
    destroy_context();

#if JOJ_DEBUG_MODE
    if (m_debug)
    {
        // Report live objects
        m_debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
        m_debug->Release();
        m_debug = nullptr;
    }
#endif // JOJ_DEBUG_MODE
}

joj::ErrorCode joj::D3D11Renderer::create_context()
{
    // ---------------------------------------------------
    // Create DXGI infrastructure and D3D device
    // ---------------------------------------------------

    u32 factory_flags = 0;
    u32 create_device_flags = 0;

    factory_flags = DXGI_CREATE_FACTORY_DEBUG;

#if JOJ_DEBUG_MODE
    create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // JOJ_DEBUG_MODE

    if (CreateDXGIFactory2(factory_flags, IID_PPV_ARGS(&m_factory)) != S_OK)
    {
        JOJ_FATAL(ErrorCode::ERR_CONTEXT_CREATE_DXGI_FACTORY2,
            "Failed to create D3D11 DXGIFactory2.");
        return ErrorCode::ERR_CONTEXT_CREATE_DXGI_FACTORY2;
    }

    // Level of D3D features supported by hardware
    D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;

    if (D3D11CreateDevice(
        NULL,                            // Default adapter
        D3D_DRIVER_TYPE_HARDWARE,        // D3D driver type (Hardware, Reference or Software)
        NULL,                            // Pointer to software rasterizer - No software device
        create_device_flags,             // Debug or Normal Mode
        NULL,                            // Default feature level (NULL = max supported)
        0,                               // Size of feature level array
        D3D11_SDK_VERSION,               // Direct3D SDK version
        &m_graphics_device.device,                  // Stores D3D device created
        &feature_level,                  // Current Direct3D version in use,
        &m_cmd_list.device_context)          // D3D context device
        != S_OK)
    {
        JOJ_ERROR(ErrorCode::ERR_CONTEXT_D3D11_CREATE_DEVICE,
            "Failed to create D3D11Device. Creating D3D11 Warp adapter...");

        if (D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_WARP,
            NULL, create_device_flags, NULL, 0, D3D11_SDK_VERSION,
            &m_graphics_device.device, &feature_level, &m_cmd_list.device_context) != S_OK)
        {
            JOJ_FATAL(ErrorCode::ERR_CONTEXT_D3D11_CREATE_DEVICE_WARP,
                "Failed to create WARP Adapter.");
            return ErrorCode::ERR_CONTEXT_D3D11_CREATE_DEVICE_WARP;
        }
    }

#if JOJ_DEBUG_MODE
    if (m_graphics_device.device->QueryInterface(__uuidof(ID3D11Debug), (void**)&m_debug) != S_OK)
    {
        JOJ_ERROR(ErrorCode::ERR_CONTEXT_QUERY_INTERFACE_ID3D11_DEBUG,
            "Failed to QueryInterface of ID3D11Debug.");
    }
#endif // JOJ_DEBUG_MODE

    IDXGIDevice* dxgi_device = nullptr;
    if (m_graphics_device.device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgi_device) != S_OK)
    {
        JOJ_FATAL(ErrorCode::ERR_CONTEXT_QUERY_INTERFACE_IDXGI_DEVICE,
            "Failed to QueryInterface of DXGIDevice.");
        return ErrorCode::ERR_CONTEXT_QUERY_INTERFACE_IDXGI_DEVICE;
    }

    // Get Adpapter from Direct3D device (d3d11Device)
    IDXGIAdapter* dxgi_adapter = nullptr;
    if (dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgi_adapter) != S_OK)
    {
        JOJ_FATAL(ErrorCode::ERR_CONTEXT_GET_PARENTOF_IDXGI_ADAPTER,
            "Failed to GetParent of IDXGIAdapter.");
        return ErrorCode::ERR_CONTEXT_GET_PARENTOF_IDXGI_ADAPTER;
    }

    // Get pointer to adapter DXGIFactory
    IDXGIFactory2* dxgi_factory = nullptr;
    if (dxgi_adapter->GetParent(__uuidof(IDXGIFactory2), (void**)&dxgi_factory) != S_OK)
    {
        JOJ_FATAL(ErrorCode::ERR_CONTEXT_GET_PARENTOF_IDXGI_FACTORY2,
            "Failed to GetParent of IDXGIFactory2.");
        return ErrorCode::ERR_CONTEXT_GET_PARENTOF_IDXGI_FACTORY2;
    }

#ifdef JOJ_DEBUG_MODE
    log_hardware_info();
#endif // JOJ_DEBUG_MODE

    dxgi_device->Release();
    dxgi_adapter->Release();
    dxgi_factory->Release();

    return ErrorCode::OK;
}

void joj::D3D11Renderer::destroy_context()
{
    // Release factory
    if (m_factory)
    {
        m_factory->Release();
        m_factory = nullptr;
    }

    // Release device context
    if (m_cmd_list.device_context)
    {
        // Restores to original state
        m_cmd_list.device_context->ClearState();
        m_cmd_list.device_context->Release();
        m_cmd_list.device_context = nullptr;
    }

    // Release device
    if (m_graphics_device.device)
    {
        m_graphics_device.device->Release();
        m_graphics_device.device = nullptr;
    }
}

joj::ErrorCode joj::D3D11Renderer::initialize(WindowData* window)
{
    JOJ_ASSERT(window, "WindowData is nullptr.");

    if (create_context() != ErrorCode::OK)
    {
        JOJ_FATAL(ErrorCode::ERR_CONTEXT_D3D11_CREATION,
            "Failed to create D3D11 context.");
        return ErrorCode::ERR_CONTEXT_D3D11_CREATION;
    }

    if (m_graphics_device.device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM,
        4, &m_4xmsaa_quality) != S_OK)
    {
        // TODO: Better ErrorCode
        JOJ_ERROR(ErrorCode::ERR_RENDERER_CHECK_MULTISAMPLE_QUALITY_LEVELS,
            "Failed to check multi sample quality levels. Setting it to 0.");
        m_4xmsaa_quality = 0;
    }

    // ------------------------------------------------------------------------------------------------------
    //                                          PIPELINE SETUP
    // ------------------------------------------------------------------------------------------------------

    // ---------------------------------------------------
    // SwapChain
    // ---------------------------------------------------

    // Describe Swap Chain
    DXGI_SWAP_CHAIN_DESC swap_chain_desc = { 0 };
    swap_chain_desc.BufferDesc.Width = window->width;                      // Back buffer width
    swap_chain_desc.BufferDesc.Height = window->height;                    // Back buffer height
    swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;                                  // Refresh rate in hertz 
    swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;                                 // Denominator is an int
    swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;                         // Color format - RGBA 8 bits
    swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;     // Default value for Flags
    swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;                     // Default mode for scaling

    // Using 4x MSAA
    if (m_4xmsaa_enabled)
    {
        swap_chain_desc.SampleDesc.Count = 4;                                               // Samples per pixel (antialiasing)
        swap_chain_desc.SampleDesc.Quality = m_4xmsaa_quality - 1;                          // Level of image quality
    }
    // No MSAA
    else
    {
        swap_chain_desc.SampleDesc.Count = 1;
        swap_chain_desc.SampleDesc.Quality = 0;
    }

    swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;                          // Use surface as Render Target

    // Check value
    swap_chain_desc.BufferCount = m_buffer_count;                                           // Number of buffers (Front + Back)

    swap_chain_desc.OutputWindow = window->handle;                                           // Window ID
    swap_chain_desc.Windowed = (window->window_mode == joj::WindowMode::Windowed);           // Fullscreen or windowed 
    swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;                             // Discard surface after presenting
    swap_chain_desc.Flags = 0;                                                              // Use Back buffer size for Fullscreen

    // Create Swap Chain
    if (m_factory->CreateSwapChain(m_graphics_device.device, &swap_chain_desc,
        &m_swapchain) != S_OK)
    {
        JOJ_FATAL(ErrorCode::ERR_RENDERER_SWAPCHAIN_CREATION,
            "Failed to create SwapChain.");
        return ErrorCode::ERR_RENDERER_SWAPCHAIN_CREATION;
    }

    // ---------------------------------------------------
    // Render Target View
    // ---------------------------------------------------

    // Get backbuffer surface of a Swap Chain
    ID3D11Texture2D* backbuffer = nullptr;
    if (m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D),
        reinterpret_cast<void**>(&backbuffer)) != S_OK)
    {
        JOJ_FATAL(ErrorCode::ERR_RENDERER_SWAPCHAIN_GET_BACKBUFFER,
            "Failed to Get backbuffer surface of a Swap Chain.");
        return ErrorCode::ERR_RENDERER_SWAPCHAIN_GET_BACKBUFFER;
    }

    // Create render target view for backbuffer
    if (m_graphics_device.device->CreateRenderTargetView(backbuffer, NULL,
        &m_render_target_view) != S_OK)
    {
        JOJ_FATAL(ErrorCode::ERR_RENDERER_CREATE_RENDER_TARGET_VIEW,
            "Failed to create D3D11 Render Target View.");
        return ErrorCode::ERR_RENDERER_CREATE_RENDER_TARGET_VIEW;
    }

    // ---------------------------------------------------
    // Depth/Stencil Buffer
    // ---------------------------------------------------

    // Describe Depth/Stencil Buffer
    D3D11_TEXTURE2D_DESC depth_stencil_tex2d_desc = { 0 };
    depth_stencil_tex2d_desc.Width = static_cast<u32>(window->width);           // Depth/Stencil buffer width
    depth_stencil_tex2d_desc.Height = static_cast<u32>(window->height);         // Depth/Stencil buffer height
    depth_stencil_tex2d_desc.MipLevels = 0;                                    // Number of mipmap levels
    depth_stencil_tex2d_desc.ArraySize = 1;                                    // Number of textures in array
    depth_stencil_tex2d_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;           // Color format - Does it need to be the same format of swapChainDesc?

    // Use 4x MSAA? It must match swap chain MSAA values.
    if (m_4xmsaa_enabled)
    {
        depth_stencil_tex2d_desc.SampleDesc.Count = 4;                         // Samples per pixel (antialiasing)
        depth_stencil_tex2d_desc.SampleDesc.Quality = m_4xmsaa_quality - 1;    // Level of image quality
    }
    // No MSAA
    else
    {
        depth_stencil_tex2d_desc.SampleDesc.Count = 1;                         // Samples per pixel (antialiasing)
        depth_stencil_tex2d_desc.SampleDesc.Quality = 0;                       // Level of image quality
    }

    depth_stencil_tex2d_desc.Usage = D3D11_USAGE_DEFAULT;                      // Default - GPU will both read and write to the resource
    depth_stencil_tex2d_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;             // Where resource will be bound to the pipeline
    depth_stencil_tex2d_desc.CPUAccessFlags = 0;                               // CPU will not read not write to the Depth/Stencil buffer
    depth_stencil_tex2d_desc.MiscFlags = 0;                                    // Optional flags

    // Create Depth/Stencil Buffer
    if (m_graphics_device.device->CreateTexture2D(&depth_stencil_tex2d_desc, 0,
        &m_depth_stencil_buffer) != S_OK)
    {
        JOJ_FATAL(ErrorCode::ERR_RENDERER_CREATE_TEXTURE2D,
            "Failed to create D3D11 DepthStencil buffer (Texture2D).");
        return ErrorCode::ERR_RENDERER_CREATE_TEXTURE2D;
    }

    // ---------------------------------------------------
    // Depth/Stencil State - Depth Enabled
    // ---------------------------------------------------

    // Describe Depth Stencil
    D3D11_DEPTH_STENCIL_DESC depth_stencil_desc = { 0 };

    // Set up the description of the stencil state.
    depth_stencil_desc.DepthEnable = true;
    depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;

    depth_stencil_desc.StencilEnable = true;
    depth_stencil_desc.StencilReadMask = 0xFF;
    depth_stencil_desc.StencilWriteMask = 0xFF;

    // Stencil operations if pixel is front-facing.
    depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Stencil operations if pixel is back-facing.
    depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Create Depth Stencil State
    if (m_graphics_device.device->CreateDepthStencilState(&depth_stencil_desc,
        &m_depth_stencil_state) != S_OK)
    {
        JOJ_FATAL(ErrorCode::ERR_RENDERER_CREATE_DEPTHSTENCIL_STATE,
            "Failed to create D3D11 Depth Stencil State.");
        return ErrorCode::ERR_RENDERER_CREATE_DEPTHSTENCIL_STATE;
    }

    // Set Depth Stencil State
    m_cmd_list.device_context->OMSetDepthStencilState(m_depth_stencil_state, 1);

    // ---------------------------------------------------
    // Depth/Stencil State - Depth Disabled
    // ---------------------------------------------------

    // Describe Depth Stencil
    D3D11_DEPTH_STENCIL_DESC depth_disabled_stencil_desc = { 0 };
    ZeroMemory(&depth_disabled_stencil_desc, sizeof(depth_disabled_stencil_desc));

    // Create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
    // that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
    depth_disabled_stencil_desc.DepthEnable = false;
    depth_disabled_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depth_disabled_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;
    depth_disabled_stencil_desc.StencilEnable = true;
    depth_disabled_stencil_desc.StencilReadMask = 0xFF;
    depth_disabled_stencil_desc.StencilWriteMask = 0xFF;
    depth_disabled_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depth_disabled_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depth_disabled_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depth_disabled_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    depth_disabled_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depth_disabled_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depth_disabled_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depth_disabled_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    
    // Create Depth Stencil State
    if (m_graphics_device.device->CreateDepthStencilState(&depth_disabled_stencil_desc,
        &m_depth_disabled_stencil_state) != S_OK)
    {
        JOJ_FATAL(ErrorCode::ERR_RENDERER_CREATE_DEPTHSTENCIL_STATE,
            "Failed to create D3D11 Depth Stencil State for disabled depth.");
        return ErrorCode::ERR_RENDERER_CREATE_DEPTHSTENCIL_STATE;
    }

    // ---------------------------------------------------
    // Depth/Stencil View
    // ---------------------------------------------------

    D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc = {};
    ZeroMemory(&depth_stencil_view_desc, sizeof(depth_stencil_view_desc));

    // Set up the depth stencil view description.
    depth_stencil_view_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depth_stencil_view_desc.Texture2D.MipSlice = 0;

    // Create Depth/Stencil View
    if (m_graphics_device.device->CreateDepthStencilView(m_depth_stencil_buffer,
        &depth_stencil_view_desc, &m_depth_stencil_view) != S_OK)
    {
        JOJ_FATAL(ErrorCode::ERR_RENDERER_CREATE_DEPTHSTENCIL_VIEW,
            "Failed to create D3D11 Depth Stencil View.");
        return ErrorCode::ERR_RENDERER_CREATE_DEPTHSTENCIL_VIEW;
    }

    // Bind render target and depth stencil to the Output Merger stage
    m_cmd_list.device_context->OMSetRenderTargets(
        1, &m_render_target_view, m_depth_stencil_view);

    // ---------------------------------------------------
    // Viewport
    // ---------------------------------------------------

    // Describe Viewport
    m_viewport.TopLeftX = 0.0f;
    m_viewport.TopLeftY = 0.0f;
    m_viewport.Width = static_cast<f32>(window->width);
    m_viewport.Height = static_cast<f32>(window->height);
    m_viewport.MinDepth = 0.0f;
    m_viewport.MaxDepth = 1.0f;

    // Set Viewport
    m_cmd_list.device_context->RSSetViewports(1, &m_viewport);

    // ---------------------------------------------
    // Blend State
    // ---------------------------------------------

    // Describe blend state
    D3D11_BLEND_DESC blend_desc = { 0 };
    blend_desc.AlphaToCoverageEnable = false;                                // Highlight the silhouette of sprites
    blend_desc.IndependentBlendEnable = false;                               // Use the same mix for all render targets
    blend_desc.RenderTarget[0].BlendEnable = true;                           // Enable blending
    blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;             // Source mixing factor
    blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;        // Target of RGB mixing is inverted alpha
    blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;                 // Addition operation in color mixing
    blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;        // Alpha blend source is the alpha of the pixel shader
    blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;   // Fate of Alpha mixture is inverted alpha
    blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;            // Addition operation in color mixing
    blend_desc.RenderTarget[0].RenderTargetWriteMask = 0x0F;                 // Components of each pixel that can be overwritten

    // Create blend state
    if (m_graphics_device.device->CreateBlendState(&blend_desc, &m_blend_state) != S_OK)
    {
        JOJ_FATAL(ErrorCode::ERR_RENDERER_CREATE_BLENDSTATE,
            "Failed to create D3D11 BlendState.");
        return ErrorCode::ERR_RENDERER_CREATE_BLENDSTATE;
    }

    // Bind blend state to the Output Merger stage
    m_cmd_list.device_context->OMSetBlendState(m_blend_state, nullptr, 0xffffffff);

    // ---------------------------------------------------
    // Rasterizer
    // ---------------------------------------------------

    // Describe rasterizer
    D3D11_RASTERIZER_DESC rasterizer_desc = { };
    ZeroMemory(&rasterizer_desc, sizeof(rasterizer_desc));
    rasterizer_desc.FillMode = D3D11_FILL_SOLID;
    rasterizer_desc.CullMode = D3D11_CULL_BACK;
    rasterizer_desc.DepthClipEnable = true;

    // Create Solid rasterizer state
    if (m_graphics_device.device->CreateRasterizerState(&rasterizer_desc, &m_rasterizer_state_solid) != S_OK)
    {
        JOJ_FATAL(ErrorCode::ERR_RENDERER_CREATE_RASTERIZERSTATE,
            "Failed to create Solid D3D11 RasterizerState.");
        return ErrorCode::ERR_RENDERER_CREATE_RASTERIZERSTATE;
    }

    rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
    rasterizer_desc.CullMode = D3D11_CULL_BACK;

    // Create Wireframe rasterizer state
    if (m_graphics_device.device->CreateRasterizerState(&rasterizer_desc, &m_rasterizer_state_wireframe) != S_OK)
    {
        JOJ_FATAL(ErrorCode::ERR_RENDERER_CREATE_RASTERIZERSTATE,
            "Failed to create Wireframe D3D11 RasterizerState.");
        return ErrorCode::ERR_RENDERER_CREATE_RASTERIZERSTATE;
    }

    // Set Solid rasterizer state as default
    m_cmd_list.device_context->RSSetState(m_rasterizer_state_solid);

    set_primitive_topology(PrimitiveTopology::TRIANGLE_LIST);

    // ---------------------------------------------------
    //	Release Resources
    // ---------------------------------------------------

    backbuffer->Release();

    return ErrorCode::OK;
}

void joj::D3D11Renderer::shutdown()
{
    // Release wireframe rasterizer state
    if (m_rasterizer_state_wireframe)
    {
        m_rasterizer_state_wireframe->Release();
        m_rasterizer_state_wireframe = nullptr;
    }

    // Release solid rasterizer state
    if (m_rasterizer_state_solid)
    {
        m_rasterizer_state_solid->Release();
        m_rasterizer_state_solid = nullptr;
    }

    // Release blend state
    if (m_blend_state)
    {
        m_blend_state->Release();
        m_blend_state = nullptr;
    }

    // Release depth stencil view
    if (m_depth_stencil_view)
    {
        m_depth_stencil_view->Release();
        m_depth_stencil_view = nullptr;
    }

    // Release render target view
    if (m_render_target_view)
    {
        m_render_target_view->Release();
        m_render_target_view = nullptr;
    }

    // Release swap chain
    if (m_swapchain)
    {
        // Direct3D is unable to close when full screen
        m_swapchain->SetFullscreenState(false, NULL);
        m_swapchain->Release();
        m_swapchain = nullptr;
    }

    // Release factory
    if (m_factory)
    {
        m_factory->Release();
        m_factory = nullptr;
    }

    // Release device context
    if (m_cmd_list.device_context)
    {
        // Restores to original state
        m_cmd_list.device_context->ClearState();
        m_cmd_list.device_context->Release();
        m_cmd_list.device_context = nullptr;
    }

    // Release device
    if (m_graphics_device.device)
    {
        m_graphics_device.device->Release();
        m_graphics_device.device = nullptr;
    }
}

void joj::D3D11Renderer::begin_frame()
{
    JOJ_ASSERT(m_cmd_list.device_context, "Device context is nullptr.");
    JOJ_ASSERT(m_render_target_view, "Render target view is nullptr.");
    JOJ_ASSERT(m_depth_stencil_view, "Depth stencil view is nullptr.");

    // Background color of the backbuffer = window background color
    f32 bgcolor[4]{ 0.23f, 0.23f, 0.23f, 1.0f };
    m_cmd_list.device_context->ClearRenderTargetView(m_render_target_view, bgcolor);
    m_cmd_list.device_context->ClearDepthStencilView(m_depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void joj::D3D11Renderer::end_frame()
{
    JOJ_ASSERT(m_swapchain, "SwapChain is nullptr.");
    JOJ_ASSERT(m_cmd_list.device_context, "Device context is nullptr.");

    if (m_swapchain->Present(m_vsync, NULL) != S_OK)
    {
        JOJ_ERROR(ErrorCode::ERR_RENDERER_SWAPCHAIN_PRESENT,
            "Failed to present SwapChain.");
        return;
    }

    m_cmd_list.device_context->OMSetRenderTargets(1, &m_render_target_view, m_depth_stencil_view);
}

void joj::D3D11Renderer::draw(const u32 vertex_count, const u32 start_vertex_location)
{
    JOJ_ASSERT(m_cmd_list.device_context, "Device context is nullptr.");

    m_cmd_list.device_context->Draw(vertex_count, start_vertex_location);
}

void joj::D3D11Renderer::draw_indexed(const u32 index_count, const u32 start_index_location,
    const i32 base_vertex_location)
{
    JOJ_ASSERT(m_cmd_list.device_context, "Device context is nullptr.");

    m_cmd_list.device_context->DrawIndexed(index_count, start_index_location, base_vertex_location);
}

void joj::D3D11Renderer::draw_rect(const i32 x, const i32 y, const i32 width,
    const i32 height, const Color* color)
{
    JOJ_ASSERT(m_cmd_list.device_context, "Device context is nullptr.");
    JOJ_ASSERT(m_graphics_device.device, "Device is nullptr.");

    // Hard coded screen width and height
    // TODO: Use screen_width and screen_height from WindowData
    constexpr i32 screen_width = 800;
    constexpr i32 screen_height = 600;

    // Normalize coordinates
    const f32 left = (2.0f * x) / static_cast<f32>(screen_width) - 1.0f;
    const f32 right = (2.0f * (x + width)) / static_cast<f32>(screen_width) - 1.0f;
    const f32 top = 1.0f - (2.0f * y) / static_cast<f32>(screen_height);
    const f32 bottom = 1.0f - (2.0f * (y + height)) / static_cast<f32>(screen_height);

    // Color
    FloatColor float_color;
    Vector4 color_array;
    if (color == nullptr)
    {
        float_color = Color::White.to_FloatColor();
        color_array = { float_color.r, float_color.g, float_color.b, float_color.a };
    }
    else
    {
        float_color = color->to_FloatColor();
        color_array = { float_color.r, float_color.g, float_color.b, float_color.a };
    }

    Vertex::RectUIType vertices[] = 
    {
        { Vector3{ left,  bottom, 0.0f }, color_array }, // 2: Bottom-left
        { Vector3{ right, bottom, 0.0f }, color_array }, // 3: Bottom-right
        { Vector3{ left,  top,    0.0f }, color_array }, // 0: Top-left
        { Vector3{ right, top,    0.0f }, color_array }, // 1: Top-right
    };

    u16 indices[] =
    {
        0, 2, 1,  // First triangle (CCW)
        1, 2, 3   // Second triangle (CCW)
    };

    // Create vertex buffer
    D3D11VertexBuffer vb(&m_graphics_device, &m_cmd_list);
    if (vb.create(BufferUsage::Default, CPUAccessType::None, sizeof(Vertex::RectUIType) * 4, vertices) != ErrorCode::OK)
        return;

    // Create index buffer
    D3D11IndexBuffer ib(&m_graphics_device, &m_cmd_list);
    if (ib.create(BufferUsage::Default, CPUAccessType::None, sizeof(u16) * 6, indices) != ErrorCode::OK)
        return;

    // Create shader
    D3D11Shader shader(&m_graphics_device, &m_cmd_list);
    if (shader.compile_vertex_shader(ShaderLibrary::SimpleColor, "VS", ShaderModel::Default) != ErrorCode::OK)
        return;
    
    if (shader.compile_pixel_shader(ShaderLibrary::SimpleColor, "PS", ShaderModel::Default) != ErrorCode::OK)
        return;

    std::vector<InputDesc> layout =
    {
        { "POSITION", 0, DataFormat::R32G32B32_FLOAT,    0,  0, InputClassification::PerVertexData, 0 },
        { "COLOR",    0, DataFormat::R32G32B32A32_FLOAT, 0, 12, InputClassification::PerVertexData, 0 }
    };

    if (shader.create_input_layout(layout) != ErrorCode::OK)
        return;

    // Bind and Draw the rect
    constexpr u32 stride = sizeof(Vertex::RectUIType);
    constexpr u32 offset = 0;
    vb.bind(0, 1, &stride, &offset);
    ib.bind(joj::DataFormat::R16_UINT, offset);
    shader.bind();
    draw_indexed(6, 0, 0);
}

void joj::D3D11Renderer::set_rasterizer_state(const RasterizerState state)
{
    JOJ_ASSERT(m_cmd_list.device_context, "Device context is nullptr.");
    JOJ_ASSERT(m_rasterizer_state_wireframe, "Wireframe rasterizer state is nullptr.");
    JOJ_ASSERT(m_rasterizer_state_solid, "Solid rasterizer state is nullptr.");

    switch (state)
    {
    case RasterizerState::Wireframe:
        m_cmd_list.device_context->RSSetState(m_rasterizer_state_wireframe);
        break;
    case RasterizerState::Solid:
        m_cmd_list.device_context->RSSetState(m_rasterizer_state_solid);
        break;
    default:
        break;
    }
}

void joj::D3D11Renderer::set_primitive_topology(const PrimitiveTopology topology)
{
    JOJ_ASSERT(m_cmd_list.device_context, "Device context is nullptr.");

    switch (topology)
    {
    case PrimitiveTopology::UNDEFINED:
        m_cmd_list.device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED);
        break;
    case PrimitiveTopology::POINT_LIST:
        m_cmd_list.device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
        break;
    case PrimitiveTopology::LINE_LIST:
        m_cmd_list.device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
        break;
    case PrimitiveTopology::LINE_STRIP:
        m_cmd_list.device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
        break;
    case PrimitiveTopology::TRIANGLE_LIST:
        m_cmd_list.device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        break;
    case PrimitiveTopology::TRIANGLE_STRIP:
        m_cmd_list.device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
        break;
    default:
        break;
    }
}

joj::GraphicsDevice* joj::D3D11Renderer::get_device()
{
    return &m_graphics_device;
}

joj::CommandList* joj::D3D11Renderer::get_cmd_list()
{
    return &m_cmd_list;
}

#if JOJ_DEBUG_MODE
void joj::D3D11Renderer::log_hardware_info()
{
    const u32 bytes_in_megabyte = 1048576U;

    // ---------------------------------------------------
    // Video adapter (Graphics card)
    // ---------------------------------------------------

    IDXGIAdapter* adapter = nullptr;
    if (m_factory->EnumAdapters(0, &adapter) != DXGI_ERROR_NOT_FOUND)
    {
        DXGI_ADAPTER_DESC desc;
        adapter->GetDesc(&desc);

        char graphics_card[128];
        size_t converted_chars = 0;
        wcstombs_s(&converted_chars, graphics_card, sizeof(graphics_card), desc.Description, _TRUNCATE);

        JOJ_INFO("---> Graphics card: %s.", graphics_card);
    }

    IDXGIAdapter4* adapter4 = nullptr;
    if (SUCCEEDED(adapter->QueryInterface(IID_PPV_ARGS(&adapter4))))
    {
        DXGI_QUERY_VIDEO_MEMORY_INFO mem_info;
        adapter4->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &mem_info);

        JOJ_INFO("---> Video memory (free): %lluMB.", mem_info.Budget / bytes_in_megabyte);

        // FIXME: Memory used is wrong
        JOJ_INFO("---> Video memory(used) : %lluMB.", mem_info.CurrentUsage / bytes_in_megabyte);

        adapter4->Release();
    }

    // ---------------------------------------------------
    // Max feature Level supported by GPU
    // ---------------------------------------------------

    // Instructions block
    {
        JOJ_INFO("---> Feature Level: 11_0.");
    }

    // ---------------------------------------------------
    // Video output (monitor)
    // ---------------------------------------------------

    IDXGIOutput* output = nullptr;
    if (adapter->EnumOutputs(0, &output) != DXGI_ERROR_NOT_FOUND)
    {
        DXGI_OUTPUT_DESC desc;
        output->GetDesc(&desc);

        char device_name[32];
        size_t converted_chars = 0;
        wcstombs_s(&converted_chars, device_name, sizeof(device_name), desc.DeviceName, _TRUNCATE);

        JOJ_INFO("---> Monitor: %s.", device_name);
    }

    // ----------------------------------------------------
    // Video mode (resolution)
    // ----------------------------------------------------

    // Get screen dimensions
    u32 dpi = GetDpiForSystem();
    u32 screen_width = GetSystemMetricsForDpi(SM_CXSCREEN, dpi);
    u32 screen_height = GetSystemMetricsForDpi(SM_CYSCREEN, dpi);

    // Get screen update frequency
    DEVMODE dev_mode = { 0 };
    dev_mode.dmSize = sizeof(DEVMODE);
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dev_mode);
    u32 refresh = dev_mode.dmDisplayFrequency;

    JOJ_INFO("---> Resolution: %dx%d %d Hz.", screen_width, screen_height, refresh);

    // Release used DXGI interfaces
    if (adapter) adapter->Release();
    if (output) output->Release();
}
#endif // JOJ_DEBUG_MODE

#endif // JOJ_PLATFORM_WINDOWS