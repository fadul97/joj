#include "joj/gfx/rhi/vk/renderer_backend.hpp"

#include "joj/core/logging/logger.hpp"

namespace joj::gfx::rhi::vk {

ErrorCode RendererBackend::initialize() noexcept
{
    JOJ_LOG_TRACE("RendererBackend initialized...\n");
    return ErrorCode::OK;
}

void RendererBackend::shutdown() noexcept
{
    JOJ_LOG_TRACE("RendererBackend shutdown...\n");
}

} // namespace joj::gfx::rhi::vk
