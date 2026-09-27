#include "joj/gfx/renderer.hpp"

#include "joj/core/assert.hpp"
#include "joj/core/logging/logger.hpp"
#include "joj/gfx/rhi/vk/renderer_backend.hpp"

namespace joj::gfx {

ErrorCode Renderer::initialize() noexcept
{
    JOJ_ASSERT(m_backend == nullptr);

    m_backend = new rhi::vk::RendererBackend{};
    JOJ_ASSERT(m_backend);

    return m_backend->initialize();
}

void Renderer::shutdown() noexcept
{
    if (m_backend)
    {
        m_backend->shutdown();
        delete m_backend;
        m_backend = nullptr;
    }
}

} // namespace joj::gfx
