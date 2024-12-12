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
    struct RendererData
    {
        ID3D11Device* device;
        ID3D11DeviceContext* device_context;
    };

    class JAPI D3D11Renderer : public IRenderer
    {
    public:
        D3D11Renderer();
        ~D3D11Renderer();

        ErrorCode create_context() override;
        void destroy_context() override;

        ErrorCode initialize() override;
        void shutdown() override;

        void resize(i32 width, i32 height) override;

        RendererData& get_data() override;

#if JOJ_DEBUG_MODE
        void log_hardware_info();
#endif // JOJ_DEBUG_MODE

    private:
        RendererData m_data;

        IDXGIFactory6* m_factory;

#if JOJ_DEBUG_MODE
        ID3D11Debug* m_debug;
#endif // JOJ_DEBUG_MODE
    };
}

#endif // JPLATFORM_WINDOWS

#endif // _JOJ_D3D11_RENDERER_H