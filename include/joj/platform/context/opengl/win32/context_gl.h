#ifndef JOJ_WIN32_GL_CONTEXT_H
#define JOJ_WIN32_GL_CONTEXT_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include "platform/context/context.h"
#include "platform/win32/window_win32.h"

namespace joj
{
    struct GLContextConfig
    {
        HGLRC shared_context;
    };

    class JAPI Win32GLContext
    {
    public:
        Win32GLContext();
        ~Win32GLContext();

        [[nodiscard]] ErrorCode create(const WindowConfig& window) const;
        ErrorCode make_current(const WindowConfig& window);
        void destroy() const;
          
    private:
        GLContextConfig m_context_config{};
        u32 m_gl_version_major;
        u32 m_gl_version_minor;
        i32 m_color_bits;
        i32 m_depth_bits;
        i32 m_pixel_format_attrib_list[19]{};
        i32 m_context_attribs[16]{};

        static void log_hardware_info();
        b8 is_extension_supported(const char *ext_list, const char *extension);
    };
}

#endif // JPLATFORM_WINDOWS

#endif // JOJ_WIN32_GL_CONTEXT_H
