#include "platform/win32/window_procedure_win32.h"

#if JPLATFORM_WINDOWS

joj::Win32WindowProcedure::Win32WindowProcedure()
{
}

joj::Win32WindowProcedure::~Win32WindowProcedure()
{
}

RESULT joj::Win32WindowProcedure::handle_message(void* hWnd, u32 message, unsigned long wParam, long lParam)
{
    return 0;
}

#endif // JPLATFORM_WINDOWS