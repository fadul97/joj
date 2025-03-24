#include "joj/core/error_code.h"

const char* joj::err_to_str(const ErrorCode err)
{
    switch (err)
    {
    case ErrorCode::OK:                                             return "OK";
    case ErrorCode::FAILED:                                         return "FAILED";
    case ErrorCode::ASSERTION_FAILED:                               return "ASSERTION_FAILED";
    case ErrorCode::ERR_WINDOW_WINDOW_DATA_NULL:                    return "ERR_WINDOW_WINDOW_DATA_NULL";
    case ErrorCode::ERR_WINDOW_GET_MODULE_HANDLE:                   return "ERR_WINDOW_GET_MODULE_HANDLE";
    case ErrorCode::ERR_WINDOW_REGISTER_CLASS_EX:                   return "ERR_WINDOW_REGISTER_CLASS_EX";
    case ErrorCode::ERR_WINDOW_CREATE_WINDOW_EX:                    return "ERR_WINDOW_CREATE_WINDOW_EX";
    case ErrorCode::ERR_WINDOW_ADJUST_WINDOW_RECT_EX:               return "ERR_WINDOW_ADJUST_WINDOW_RECT_EX";
    case ErrorCode::ERR_WINDOW_MOVE_WINDOW:                         return "ERR_WINDOW_MOVE_WINDOW";
    case ErrorCode::ERR_WINDOW_GET_DC:                              return "ERR_WINDOW_GET_DC";
    case ErrorCode::ERR_WINDOW_GET_WINDOW_RECT:                     return "ERR_WINDOW_GET_WINDOW_RECT";
    case ErrorCode::ERR_WINDOW_GET_CLIENT_RECT:                     return "ERR_WINDOW_GET_CLIENT_RECT";
    case ErrorCode::ERR_WINDOW_IMPL_CREATE:                         return "ERR_WINDOW_IMPL_CREATE";
    case ErrorCode::ERR_CONTEXT_CREATE_DXGI_FACTORY2:               return "ERR_CONTEXT_CREATE_DXGI_FACTORY2";
    case ErrorCode::ERR_CONTEXT_D3D11_CREATE_DEVICE:                return "ERR_CONTEXT_D3D11_CREATE_DEVICE";
    case ErrorCode::ERR_CONTEXT_D3D11_CREATE_DEVICE_WARP:           return "ERR_CONTEXT_D3D11_CREATE_DEVICE_WARP";
    case ErrorCode::ERR_CONTEXT_QUERY_INTERFACE_ID3D11_DEBUG:       return "ERR_CONTEXT_QUERY_INTERFACE_ID3D11_DEBUG";
    case ErrorCode::ERR_CONTEXT_QUERY_INTERFACE_IDXGI_DEVICE:       return "ERR_CONTEXT_QUERY_INTERFACE_IDXGI_DEVICE";
    case ErrorCode::ERR_CONTEXT_GET_PARENTOF_IDXGI_ADAPTER:         return "ERR_CONTEXT_GET_PARENTOF_IDXGI_ADAPTER";
    case ErrorCode::ERR_CONTEXT_GET_PARENTOF_IDXGI_FACTORY2:        return "ERR_CONTEXT_GET_PARENTOF_IDXGI_FACTORY2";
    case ErrorCode::ERR_CONTEXT_D3D11_CREATION:                     return "ERR_CONTEXT_D3D11_CREATION";
    case ErrorCode::ERR_RENDERER_CHECK_MULTISAMPLE_QUALITY_LEVELS:  return "ERR_RENDERER_CHECK_MULTISAMPLE_QUALITY_LEVELS";
    case ErrorCode::ERR_RENDERER_SWAPCHAIN_CREATION:                return "ERR_RENDERER_SWAPCHAIN_CREATION";
    case ErrorCode::ERR_RENDERER_SWAPCHAIN_GET_BACKBUFFER:          return "ERR_RENDERER_SWAPCHAIN_GET_BACKBUFFER";
    case ErrorCode::ERR_RENDERER_CREATE_RENDER_TARGET_VIEW:         return "ERR_RENDERER_CREATE_RENDER_TARGET_VIEW";
    case ErrorCode::ERR_RENDERER_CREATE_TEXTURE2D:                  return "ERR_RENDERER_CREATE_TEXTURE2D";
    case ErrorCode::ERR_RENDERER_CREATE_DEPTHSTENCIL_STATE:         return "ERR_RENDERER_CREATE_DEPTHSTENCIL_STATE";
    case ErrorCode::ERR_RENDERER_CREATE_DEPTHSTENCIL_VIEW:          return "ERR_RENDERER_CREATE_DEPTHSTENCIL_VIEW";
    case ErrorCode::ERR_RENDERER_CREATE_BLENDSTATE:                 return "ERR_RENDERER_CREATE_BLENDSTATE";
    case ErrorCode::ERR_RENDERER_CREATE_RASTERIZERSTATE:            return "ERR_RENDERER_CREATE_RASTERIZERSTATE";
    case ErrorCode::ERR_RENDERER_SWAPCHAIN_PRESENT:                 return "ERR_RENDERER_SWAPCHAIN_PRESENT";
    case ErrorCode::ERR_SHADER_VERTEX_COMPILATION:                  return "ERR_SHADER_VERTEX_COMPILATION";
    case ErrorCode::ERR_SHADER_PIXEL_COMPILATION:                   return "ERR_SHADER_PIXEL_COMPILATION";
    case ErrorCode::ERR_SHADER_VERTEX_CREATION:                     return "ERR_SHADER_VERTEX_CREATION";
    case ErrorCode::ERR_SHADER_PIXEL_CREATION:                      return "ERR_SHADER_PIXEL_CREATION";
    case ErrorCode::ERR_SHADER_INPUT_LAYOUT_CREATION:               return "ERR_SHADER_INPUT_LAYOUT_CREATION";
    case ErrorCode::ERR_VERTEX_BUFFER_CREATION:                     return "ERR_VERTEX_BUFFER_CREATION";
    case ErrorCode::ERR_VERTEX_BUFFER_UPDATE:                       return "ERR_VERTEX_BUFFER_UPDATE";
    case ErrorCode::ERR_VERTEX_MAP_FAILED:                          return "ERR_VERTEX_MAP_FAILED";
    case ErrorCode::ERR_INDEX_BUFFER_CREATION:                      return "ERR_INDEX_BUFFER_CREATION";
    case ErrorCode::ERR_INDEX_BUFFER_UPDATE:                        return "ERR_INDEX_BUFFER_UPDATE";
    case ErrorCode::ERR_INDEX_MAP_FAILED:                           return "ERR_INDEX_MAP_FAILED";
    case ErrorCode::ERR_CONSTANT_BUFFER_CREATION:                   return "ERR_CONSTANT_BUFFER_CREATION";
    case ErrorCode::ERR_CONSTANT_BUFFER_UPDATE:                     return "ERR_CONSTANT_BUFFER_UPDATE";
    case ErrorCode::ERR_CONSTANT_MAP_FAILED:                        return "ERR_CONSTANT_MAP_FAILED";
    default:                                                        return "UNKNOWN";
    }
}
