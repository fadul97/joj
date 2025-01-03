#include "platform/timer.h"

joj::Timer::Timer()
    : m_stopped{false}, m_cumulative_elapsed{0.0f}
{
}

joj::Timer::~Timer()
{
}