#include "renderer/d3d11/renderer_d3d11.h"

#if JPLATFORM_WINDOWS

#include "logger.h"

joj::D3D11Renderer::D3D11Renderer()
{
    m_data.device = nullptr;
    m_data.device_context = nullptr;
    m_factory = nullptr;

#if JOJ_DEBUG_MODE
    m_debug = nullptr;
#endif // JOJ_DEBUG_MODE
}

joj::D3D11Renderer::~D3D11Renderer()
{
    // Release factory
    if (m_factory)
    {
        m_factory->Release();
        m_factory = nullptr;
    }

    // Release device context
    if (m_data.device_context)
    {
        // Restores to original state
        m_data.device_context->ClearState();
        m_data.device_context->Release();
        m_data.device_context = nullptr;
    }

    // Release device
    if (m_data.device)
    {
        m_data.device->Release();
        m_data.device = nullptr;
    }

#if JOJ_DEBUG_MODE
    // Report live objects
    m_debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
    m_debug->Release();
    m_debug = nullptr;
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
        JFATAL(ErrorCode::ERR_CONTEXT_D3D11_DXGI_FACTORY2_CREATION,
            "Failed to create D3D11 DXGIFactory2.");
        return ErrorCode::ERR_CONTEXT_D3D11_DXGI_FACTORY2_CREATION;
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
        &m_data.device,                  // Stores D3D device created
        &feature_level,                  // Current Direct3D version in use,
        &m_data.device_context)          // D3D context device
        != S_OK)
    {
        JERROR(ErrorCode::ERR_CONTEXT_D3D11_DEVICE_CREATION,
            "Failed to create D3D11Device. Creating D3D11 Warp adapter...");

        if (D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_WARP,
            NULL, create_device_flags, NULL, 0, D3D11_SDK_VERSION,
            &m_data.device, &feature_level, &m_data.device_context) != S_OK)
        {
            JFATAL(ErrorCode::ERR_CONTEXT_D3D11_WARP_DEVICE_ADAPTER_CREATION,
                "Failed to create WARP Adapter.");
            return ErrorCode::ERR_CONTEXT_D3D11_WARP_DEVICE_ADAPTER_CREATION;
        }
    }

#if JOJ_DEBUG_MODE
    if (m_data.device->QueryInterface(__uuidof(ID3D11Debug), (void**)&m_debug) != S_OK)
    {
        JERROR(ErrorCode::ERR_CONTEXT_D3D11_QUERY_INTERFACE_ID3D11_DEBUG, "Failed to QueryInterface of ID3D11Debug.");
    }
#endif // JOJ_DEBUG_MODE

    IDXGIDevice* dxgi_device = nullptr;
    if (m_data.device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgi_device) != S_OK)
    {
        JFATAL(ErrorCode::ERR_CONTEXT_D3D11_QUERY_INTERFACE_IDXGI_DEVICE,
            "Failed to QueryInterface of DXGIDevice.");
        return ErrorCode::ERR_CONTEXT_D3D11_QUERY_INTERFACE_IDXGI_DEVICE;
    }

    // Get Adpapter from Direct3D device (d3d11Device)
    IDXGIAdapter* dxgi_adapter = nullptr;
    if (dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgi_adapter) != S_OK)
    {
        JFATAL(ErrorCode::ERR_CONTEXT_D3D11_GET_PARENTOF_IDXGI_ADAPTER,
            "Failed to GetParent of IDXGIAdapter.");
        return ErrorCode::ERR_CONTEXT_D3D11_GET_PARENTOF_IDXGI_ADAPTER;
    }

    // Get pointer to adapter DXGIFactory
    IDXGIFactory2* dxgi_factory = nullptr;
    if (dxgi_adapter->GetParent(__uuidof(IDXGIFactory2), (void**)&dxgi_factory) != S_OK)
    {
        JFATAL(ErrorCode::ERR_CONTEXT_D3D11_GET_PARENTOF_IDXGI_FACTORY,
            "Failed to GetParent of IDXGIFactory2.");
        return ErrorCode::ERR_CONTEXT_D3D11_GET_PARENTOF_IDXGI_FACTORY;
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
    if (m_data.device_context)
    {
        // Restores to original state
        m_data.device_context->ClearState();
        m_data.device_context->Release();
        m_data.device_context = nullptr;
    }

    // Release device
    if (m_data.device)
    {
        m_data.device->Release();
        m_data.device = nullptr;
    }
}

joj::ErrorCode joj::D3D11Renderer::initialize()
{
    JTODO();
    return ErrorCode::FAILED;
}

void joj::D3D11Renderer::shutdown()
{
    JTODO();
}

void joj::D3D11Renderer::resize(i32 width, i32 height)
{
    JTODO();
}

joj::RendererData& joj::D3D11Renderer::get_data()
{
    return m_data;
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

        JINFO("---> Graphics card: %s.", graphics_card);
    }

    IDXGIAdapter4* adapter4 = nullptr;
    if (SUCCEEDED(adapter->QueryInterface(IID_PPV_ARGS(&adapter4))))
    {
        DXGI_QUERY_VIDEO_MEMORY_INFO mem_info;
        adapter4->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &mem_info);

        JINFO("---> Video memory (free): %lluMB.", mem_info.Budget / bytes_in_megabyte);

        // FIXME: Memory used is wrong
        JINFO("---> Video memory(used) : %lluMB.", mem_info.CurrentUsage / bytes_in_megabyte);

        adapter4->Release();
    }

    // ---------------------------------------------------
    // Max feature Level supported by GPU
    // ---------------------------------------------------

    // Instructions block
    {
        JINFO("---> Feature Level: 11_0.");
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

        JINFO("---> Monitor: %s.", device_name);
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

    JINFO("---> Resolution: %dx%d %d Hz.", screen_width, screen_height, refresh);

    // Release used DXGI interfaces
    if (adapter) adapter->Release();
    if (output) output->Release();
}
#endif // JOJ_DEBUG_MODE

#endif // JPLATFORM_WINDOWS