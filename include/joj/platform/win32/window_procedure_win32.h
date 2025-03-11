#ifndef _JOJ_WIN32_WINDOW_PROCEDURE_H
#define _JOJ_WIN32_WINDOW_PROCEDURE_H

#include "joj/core/defines.h"

#if JOJ_PLATFORM_WINDOWS

#include "platform/window_procedure.h"

namespace joj
{
    class JOJ_API Win32WindowProcedure : public WindowProcedureBase
    {
    public:
        Win32WindowProcedure();
        ~Win32WindowProcedure();

        JRESULT handle_message(void* hWnd, u32 message, unsigned long wParam, long lParam) override;
    };
}

#endif // JOJ_PLATFORM_WINDOWS

#endif // _JOJ_WIN32_WINDOW_PROCEDURE_H