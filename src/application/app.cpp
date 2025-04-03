#include "joj/application/app.h"

#if JOJ_PLATFORM_WINDOWS

joj::App::App()
    : m_window(nullptr), m_input(nullptr),
    m_timer(nullptr), m_renderer(nullptr)
{
}

joj::App::~App()
{
}

#endif // JOJ_PLATFORM_WINDOWS