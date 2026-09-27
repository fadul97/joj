#ifndef _JOJ_GFX_RENDERER_HPP
#define _JOJ_GFX_RENDERER_HPP

#include "joj/core/error/error_code.hpp"
#include "joj/core/typedefs.h"
#include "joj/gfx/rhi/typedefs.hpp"

namespace joj::gfx {

class Renderer {
    JOJ_MAKE_DEFAULT_CTORS_AND_DTORS(Renderer);

public:
    ErrorCode initialize(DisplayServer const* const display_server) noexcept;
    void shutdown() noexcept;

private:
    rhi::vk::RendererBackend* m_backend{ nullptr };
};

} // namespace joj::gfx

#endif // _JOJ_GFX_RENDERER_HPP
