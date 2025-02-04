#ifndef _JOJ_WINDOW_PROCEDURE_H
#define _JOJ_WINDOW_PROCEDURE_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <functional>

#if JPLATFORM_WINDOWS
typedef __int64 RESULT;
#elif JPLATFORM_LINUX
typedef long RESULT;
#endif

namespace joj
{
    using WindowProcedure = std::function<void(void*, u32, unsigned long, long)>;

    class JAPI WindowProcedureBase
    {
    public:
        WindowProcedureBase() = default;
        virtual ~WindowProcedureBase() = default;

        virtual RESULT handle_message(void* hWnd, u32 message, unsigned long wParam, long lParam) = 0;
    };
}

#endif // _JOJ_WINDOW_PROCEDURE_H