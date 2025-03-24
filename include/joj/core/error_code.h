/**
 * @file error_code.h
 * @author Leonardo Fadul (lsffadul@gmail.com)
 * @brief This file contains the error codes used throughout the engine,
 * and some helper functions for debugging purposes.
 * @version 0.1
 * @date 2025-03-17
 *
 * @copyright TODO: Copyright (c) 2025
 *
 *
 */

#ifndef _JOJ_ERROR_CODE_H
#define _JOJ_ERROR_CODE_H

#include "defines.h"

namespace joj
{
    /**
     * Using enum class instead of regular enum for scope and type safety,
     * otherwise, I would have to check if the ErrorCode returned was actually
     * in the possible interval, e.g. from OK = 0, until ERROR_CODE_MAX (some value).
     */

    /** @brief Represents the error codes for the engine. */
    enum class ErrorCode
    {
        /** @brief OK code, should be used when the operation was successful. */
        OK,

        /** @brief Generic error code, should be used when a generic operation failed. */
        FAILED,

        /** @brief Assertion error code, should be used when an assertion failed. */
        ASSERTION_FAILED,

        // ---------------------------------------------------
        // Window creation error codes.
        // ---------------------------------------------------

        /** @brief Window creation error code, should be used when allocation of memory for window data fails. */
        ERR_WINDOW_WINDOW_DATA_NULL,

        /** @brief Win32Window creation error code, should be used for function GetModuleHandle fails. */
        ERR_WINDOW_GET_MODULE_HANDLE,

        /** @brief Win32Window creation error code, should be used when RegisterClassEx fails. */
        ERR_WINDOW_REGISTER_CLASS_EX,

        /** @brief Win32Window creation error code, should be used when CreateWindowEx fails. */
        ERR_WINDOW_CREATE_WINDOW_EX,

        /** @brief Win32Window creation error code, should be used when AdjustWindowRectEx fails. */
        ERR_WINDOW_ADJUST_WINDOW_RECT_EX,

        /** @brief Win32Window creation error code, should be used when MoveWindow fails. */
        ERR_WINDOW_MOVE_WINDOW,

        /** @brief Win32Window creation error code, should be used when GetDC fails. */
        ERR_WINDOW_GET_DC,

        /** @brief Win32Window creation error code, should be used when GetWindowRect fails. */
        ERR_WINDOW_GET_WINDOW_RECT,

        /** @brief Win32Window creation error code, should be used when GetClientRect fails. */
        ERR_WINDOW_GET_CLIENT_RECT,

        /** @brief WindowImpl creation error code, should be used when WindowImpl->create() fails. */
        ERR_WINDOW_IMPL_CREATE,

        // ---------------------------------------------------
        // Renderer creation error codes.
        // ---------------------------------------------------
        
        /** @brief D3D11Renderer creation error code, should be used when CreateDXGIFactory2 fails. */
        ERR_CONTEXT_CREATE_DXGI_FACTORY2,

        /** @brief D3D11Renderer creation error code, should be used when D3D11CreateDevice fails. */
        ERR_CONTEXT_D3D11_CREATE_DEVICE,

        /** @brief D3D11Renderer creation error code, should be used when D3D11CreateDevice fails for Warp Device. */
        ERR_CONTEXT_D3D11_CREATE_DEVICE_WARP,

        /** @brief D3D11Renderer creation error code, should be used when QueryInterface fails for ID3D11Debug. */
        ERR_CONTEXT_QUERY_INTERFACE_ID3D11_DEBUG,

        /** @brief D3D11Renderer creation error code, should be used when QueryInterface fails for IDXGIDevice. */
        ERR_CONTEXT_QUERY_INTERFACE_IDXGI_DEVICE,

        /** @brief D3D11Renderer creation error code, should be used when GetParent fails for IDXGIAdapter. */
        ERR_CONTEXT_GET_PARENTOF_IDXGI_ADAPTER,

        /** @brief D3D11Renderer creation error code, should be used when GetParent fails for IDXGIFactory2. */
        ERR_CONTEXT_GET_PARENTOF_IDXGI_FACTORY2,

        /** @brief D3D11Renderer creation error code, should be used when create_context of D3D11Renderer fails. */
        ERR_CONTEXT_D3D11_CREATION,

        /** @brief D3D11Renderer creation error code, should be used when CheckMultisampleQualityLevels fails. */
        ERR_RENDERER_CHECK_MULTISAMPLE_QUALITY_LEVELS,

        /** @brief D3D11Renderer creation error code, should be used when CreateSwapChain fails. */
        ERR_RENDERER_SWAPCHAIN_CREATION,

        /** @brief D3D11Renderer creation error code, should be used when GetBuffer fails. */
        ERR_RENDERER_SWAPCHAIN_GET_BACKBUFFER,

        /** @brief D3D11Renderer creation error code, should be used when CreateRenderTargetView fails. */
        ERR_RENDERER_CREATE_RENDER_TARGET_VIEW,

        /** @brief D3D11Renderer creation error code, should be used when CreateTexture2D fails. */
        ERR_RENDERER_CREATE_TEXTURE2D,

        /** @brief D3D11Renderer creation error code, should be used when CreateDepthStencilState fails. */
        ERR_RENDERER_CREATE_DEPTHSTENCIL_STATE,

        /** @brief D3D11Renderer creation error code, should be used when CreateDepthStencilView fails. */
        ERR_RENDERER_CREATE_DEPTHSTENCIL_VIEW,

        /** @brief D3D11Renderer creation error code, should be used when CreateBlendState fails. */
        ERR_RENDERER_CREATE_BLENDSTATE,

        /** @brief D3D11Renderer creation error code, should be used when CreateRasterizerState fails. */
        ERR_RENDERER_CREATE_RASTERIZERSTATE,

        /** @brief D3D11Renderer creation error code, should be used when SwapChain Present fails. */
        ERR_RENDERER_SWAPCHAIN_PRESENT,

        // ---------------------------------------------------
        // Shader creation error codes.
        // ---------------------------------------------------

        /** @brief D3D11Shader creation error code, should be used when compiling the vertex shader fails. */
        ERR_SHADER_VERTEX_COMPILATION,

        /** @brief D3D11Shader creation error code, should be used when compiling the pixel shader fails. */
        ERR_SHADER_PIXEL_COMPILATION,

        /** @brief D3D11Shader creation error code, should be used when creating the vertex shader fails. */
        ERR_SHADER_VERTEX_CREATION,

        /** @brief D3D11Shader creation error code, should be used when creating the pixel shader fails. */
        ERR_SHADER_PIXEL_CREATION,

        /** @brief D3D11Shader creation error code, should be used when creating the Input Layout fails. */
        ERR_SHADER_INPUT_LAYOUT_CREATION,

        // ---------------------------------------------------
        // Vertex Buffer creation error codes.
        // ---------------------------------------------------

        /** @brief D3D11VertexBuffer creation error code, should be used when creating the Vertex Buffer fails. */
        ERR_VERTEX_BUFFER_CREATION,

        /** @brief D3D11VertexBuffer update error code, should be used when updating the Vertex Buffer fails. */
        ERR_VERTEX_BUFFER_UPDATE,

        /** @brief D3D11VertexBuffer map error code, should be used when mapping the Vertex Buffer fails. */
        ERR_VERTEX_MAP_FAILED,

        // ---------------------------------------------------
        // Index Buffer creation error codes.
        // ---------------------------------------------------

        /** @brief D3D11IndexBuffer creation error code, should be used when creating the Index Buffer fails. */
        ERR_INDEX_BUFFER_CREATION,

        /** @brief D3D11IndexBuffer update error code, should be used when updating the Index Buffer fails. */
        ERR_INDEX_BUFFER_UPDATE,

        /** @brief D3D11IndexBuffer map error code, should be used when mapping the Index Buffer fails. */
        ERR_INDEX_MAP_FAILED,

        // ---------------------------------------------------
        // Constant Buffer creation error codes.
        // ---------------------------------------------------

        /** @brief D3D11ConstantBuffer creation error code, should be used when creating the Constant Buffer fails. */
        ERR_CONSTANT_BUFFER_CREATION,

        /** @brief D3D11ConstantBuffer update error code, should be used when updating the Constant Buffer fails. */
        ERR_CONSTANT_BUFFER_UPDATE,

        /** @brief D3D11ConstantBuffer map error code, should be used when mapping the Constant Buffer fails. */
        ERR_CONSTANT_MAP_FAILED,
    };

    /** @brief Get the name of the ErrorCode.
     *  @param err The error code to convert.
     *  @return The string representation of the error code.
     */
    JOJ_API const char* err_to_str(const ErrorCode err);
}

#endif // _JOJ_ERROR_CODE_H
