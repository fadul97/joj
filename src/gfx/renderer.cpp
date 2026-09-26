#include "joj/gfx/renderer.hpp"

#include "joj/core/logging/logger.hpp"

namespace joj::gfx {

ErrorCode Renderer::initialize() noexcept
{
    JOJ_LOG_TRACE("Renderer initialized...\n");
    return ErrorCode::OK;
}

void Renderer::shutdown() noexcept
{
    JOJ_LOG_TRACE("Renderer shutdown...\n");
}

} // namespace joj::gfx
