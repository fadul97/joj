#ifndef _JOJ_GFX_RENDERER_HPP
#define _JOJ_GFX_RENDERER_HPP

#include "joj/core/error/error_code.hpp"
#include "joj/core/typedefs.h"

namespace joj::gfx {

class Renderer {
    JOJ_MAKE_DEFAULT_CTORS_AND_DTORS(Renderer);

public:
    ErrorCode initialize() noexcept;
    void shutdown() noexcept;
};

} // namespace joj::gfx

#endif // _JOJ_GFX_RENDERER_HPP
