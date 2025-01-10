#ifndef _JOJ_WINDOW_FACTORY_H
#define _JOJ_WINDOW_FACTORY_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "window_registration_class.h"
#include "window.h"
#include "error_code.h"

namespace joj
{
    template <typename T>
    class JAPI WindowFactory
    {
    public:
        WindowFactory() = default;
        virtual ~WindowFactory() = default;

        virtual T* create_window(const char* title, const u32 width,
            const u32 height, const WindowMode mode) = 0;
        virtual ErrorCode create_window_class(WindowRegistrationClass& window_class) = 0;
    };
}

#endif // _JOJ_WINDOW_FACTORY_H