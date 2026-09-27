#include "joj/gfx/renderer.hpp"

#include "joj/core/assert.hpp"
#include "joj/gfx/rhi/vk/renderer_backend.hpp"

namespace joj::gfx {

ErrorCode Renderer::initialize(DisplayServer const* const display_server) noexcept
{
    JOJ_ASSERT(m_backend == nullptr);

    m_backend = new rhi::vk::RendererBackend{};
    JOJ_ASSERT(m_backend);

    return m_backend->initialize(display_server);
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
