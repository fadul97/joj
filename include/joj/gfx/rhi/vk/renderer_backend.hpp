#ifndef _JOJ_RHI_RENDERER_BACKEND_HPP
#define _JOJ_RHI_RENDERER_BACKEND_HPP

#include "joj/core/error/error_code.hpp"
#include "joj/core/typedefs.h"

namespace joj::gfx::rhi::vk {

class RendererBackend {
    JOJ_MAKE_DEFAULT_CTORS_AND_DTORS(RendererBackend);

public:
    ErrorCode initialize() noexcept;
    void shutdown() noexcept;
};

} // namespace joj::gfx::rhi::vk

#endif // _JOJ_RHI_RENDERER_BACKEND_HPP
