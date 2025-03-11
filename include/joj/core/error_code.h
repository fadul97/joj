/**
 * @file error_code.h
 * @author Leonardo Fadul ( TODO: todo@domain.com)
 * @brief This file contains the error codes used throughout the engine,
 * and some helper functions for debug purposes.
 * @version 0.1
 * @date 2024-11-13
 * 
 * @copyright TODO: Copyright (c) 2024
 * 
 * REFERENCES: Travis Vroman (https://travisvroman.com/) - Kohi Game Engine (https://github.com/travisvroman/kohi)
 * 
 */

#ifndef _JOJ_ERROR_CODE_H
#define _JOJ_ERROR_CODE_H

#include "core/defines.h"

namespace joj
{
    /** @brief Represents the error codes for the engine. */
    enum class ErrorCode
    {
        /** @brief OK code, should be used when the operation was successful. */
        OK,

        /** @brief Generic error code, should be used when the operation failed. */
        FAILED,

        /** @brief Assertion error code, should be used when an assertion fails. */
        ASSERTION_FAILED,

        /** @brief Window Error code, should be used for invalid window handle. */
        ERR_WINDOW_HANDLE,

        /** @brief Window Error code, should be used for invalid window registration. */
        ERR_WINDOW_REGISTRATION,

        /** @brief Window Error code, should be used when window fails to adjust. */
        ERR_WINDOW_ADJUST,

        /** @brief Window Error code, should be used when window fails to move. */
        ERR_WINDOW_MOVE,

        /** @brief Window Error code, should be used when the device context fails to create. */
        ERR_WINDOW_DEVICE_CONTEXT,

        /** @brief Window Error code, should be used when the window fails to get its rect. */
        ERR_WINDOW_RECT,

        /** @brief Window Error code, should be used when the window fails to get its client rect. */
        ERR_WINDOW_CLIENT_RECT,

        /** @brief D3D11 Context Error code, should be used when creation of DXGIFactory2 fails. */
        ERR_CONTEXT_D3D11_DXGI_FACTORY2_CREATION,

        /** @brief D3D11 Context Error code, should be used when creation of D3D11Device fails. */
        ERR_CONTEXT_D3D11_DEVICE_CREATION,

        /** @brief D3D11 Context Error code, should be used when creation of Warp Device Adapter fails. */
        ERR_CONTEXT_D3D11_WARP_DEVICE_ADAPTER_CREATION,

        /** @brief D3D11 Context Error code, should be used when querying ID3D11Debug fails. */
        ERR_CONTEXT_D3D11_QUERY_INTERFACE_ID3D11_DEBUG,

        /** @brief D3D11 Context Error code, should be used when querying IDXGIDevice fails. */
        ERR_CONTEXT_D3D11_QUERY_INTERFACE_IDXGI_DEVICE,

        /** @brief D3D11 Context Error code, should be used when getting parent of IDXGIAdapter fails. */
        ERR_CONTEXT_D3D11_GET_PARENTOF_IDXGI_ADAPTER,

        /** @brief D3D11 Context Error code, should be used when getting parent of IDXGIFactory fails. */
        ERR_CONTEXT_D3D11_GET_PARENTOF_IDXGI_FACTORY,

        /** @brief D3D11 Context Error code, should be used when creation of D3D11Context fails. */
        ERR_CONTEXT_D3D11_CREATION,

        /** @brief D3D11 Renderer Error code, should be used when creation of SwapChain fails. */
        ERR_SWAPCHAIN_D311_CREATION,

        /** @brief D3D11 Renderer Error code, should be used when checking for multisample quality levels fails. */
        ERR_SWAPCHAIN_D3D11_MULTISAMPLE_QUALITY_LEVELS_CHECK,

        /** @brief D3D11 Renderer Error code, should be used when getting back buffer of SwapChain fails. */
        ERR_SWAPCHAIN_D3D11_GET_BACKBUFFER,

        /** @brief D3D11 Renderer Error code, should be used when creation of RenderTargetView fails. */
        ERR_RENDER_TARGET_VIEW_D3D11_CREATION,

        /** @brief D3D11 Renderer Error code, should be used when creation of DepthStencil Texture fails. */
        ERR_DEPTHSTENCIL_BUFFER_D3D11_CREATION,

        /** @brief D3D11 Renderer Error code, should be used when creation of DepthStencil State fails. */
        ERR_DEPTHSTENCIL_STATE_D3D11_CREATION,

        /** @brief D3D11 Renderer Error code, should be used when creation of DepthStencilView fails. */
        ERR_DEPTHSTENCIL_VIEW_D3D11_CREATION,

        /** @brief D3D11 Renderer Error code, should be used when creation of BlendState fails. */
        ERR_BLENDSTATE_D3D11_CREATION,

        /** @brief D3D11 Renderer Error code, should be used when creation of RasterizerState fails. */
        ERR_RASTERIZER_D3D11_CREATION,

        /** @brief D3D11 Renderer Error code, should be used when SwapChain fails to present. */
        ERR_RENDERER_D3D11_SWAPCHAIN_PRESENT,

        /** @brief D3D11 Shader Error code, should be used when compilation of Vertex Shader fails. */
        ERR_SHADER_D3D11_VERTEX_COMPILATION,

        /** @brief D3D11 Shader Error code, should be used when compilation of Pixel Shader fails. */
        ERR_SHADER_D3D11_PIXEL_COMPILATION,

        /** @brief D3D11 Shader Error code, should be used when compilation of Geometry Shader fails. */
        ERR_SHADER_D3D11_GEOMETRY_COMPILATION,

        /** @brief D3D11 Shader Error code, should be used when compilation of Compute Shader fails. */
        ERR_SHADER_D3D11_COMPUTE_COMPILATION,

        /** @brief D3D11 Renderer code, should be used when D3D11Renderer fails to initialize. */
        ERR_RENDERER_D3D11_INIT,

        ERR_RENDER_STATE_D3D11_RASTERIZER_STATE_CREATION,
        ERR_RENDER_STATE_D3D11_BLEND_STATE_CREATION,
        ERR_RENDER_STATE_D3D11_DEPTHSTENCIL_STATE_CREATION,
        ERR_RENDERER_D3D11_TEXTURE2D_CREATION,
        ERR_RENDERER_D3D11_SHADER_RESOURCE_VIEW_CREATION,
        ERR_RENDERER_D3D11_UNORDERED_ACCESS_VIEW_CREATION,
        ERR_VERTEX_BUFFER_D3D11_CREATION,
        ERR_INDEX_BUFFER_D3D11_CREATION,
        ERR_CONSTANT_BUFFER_D3D11_CREATION,

        ERR_SHADER_D3D11_VERTEX_CREATION,
        ERR_SHADER_D3D11_PIXEL_CREATION,
        ERR_SAMPLER_STATE_D3D11_CREATION,
        ERR_INPUT_LAYOUT_D3D11_CREATION,

        ERR_FILE_OPENED_FAILED,
        ERR_FILE_LOAD_M3D_STATIC_FAILED,
        ERR_FILE_LOAD_M3D_SKINNED_FAILED,
        ERR_FILE_OBJ_OPENED_FAILED,
        ERR_TEXTURE_2D_CREATION,

        ERR_BASIC_MODEL_SET_VERTICES_FAILED,
        ERR_BASIC_MODEL_SET_INDICES_FAILED,

        ERR_GUI_CANVAS_D3D11_CREATION,
        ERR_GUI_NOT_INITIALIZED,

        ERR_GUI_BUTTON_WIN32_CREATION,
        ERR_WIN32_WINDOW_CREATE,
        ERR_WIN32_INPUT_CREATE,
        ERR_WIN32_TIMER_CREATE,
    };

    // TODO: Add all cases.
    /** @brief Get the index of the ErrorCode.
     *  @param err The error code to convert.
     *  @return The integer index of the error code.
     */
    JOJ_API i32 err_to_int(ErrorCode err);

    // TODO: Add all cases.
    /** @brief Get the name of the ErrorCode.
     *  @param err The error code to convert.
     *  @return The string representation of the error code.
     */
    JOJ_API const char* err_to_str(ErrorCode err);
}

#endif // _JOJ_ERROR_CODE_H
