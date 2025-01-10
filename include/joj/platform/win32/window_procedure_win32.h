#ifndef _JOJ_WIN32_WINDOW_PROCEDURE_H
#define _JOJ_WIN32_WINDOW_PROCEDURE_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include "platform/window_procedure.h"

namespace joj
{
    class JAPI Win32WindowProcedure : public WindowProcedureBase
    {
    public:
        Win32WindowProcedure();
        ~Win32WindowProcedure();

        RESULT handle_message(void* hWnd, u32 message, unsigned long wParam, long lParam) override;
    };
}

#endif // JPLATFORM_WINDOWS

#endif // _JOJ_WIN32_WINDOW_PROCEDURE_H