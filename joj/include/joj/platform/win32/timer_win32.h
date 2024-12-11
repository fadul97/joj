#ifndef _JOJ_WIN32_TIMER_H
#define _JOJ_WIN32_TIMER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include "platform/timer.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <timeapi.h>

namespace joj
{
    class JAPI Win32Timer : public Timer
    {
    public:
        Win32Timer();
        ~Win32Timer();

        void start();
        void stop();

        f32 reset() override;
        f32 elapsed() override;
        b8 was_elapsed(f32 secs) override;

        void begin_period();
        void end_period();

    private:
        LARGE_INTEGER m_counter_start;
        LARGE_INTEGER m_end;
        LARGE_INTEGER m_freq;
    };

    inline b8 Win32Timer::was_elapsed(f32 secs)
    { return (elapsed() >= secs ? true : false); }
    
    inline void Win32Timer::begin_period()
    { timeBeginPeriod(1); }

    inline void Win32Timer::end_period()
    { timeEndPeriod(1); }
}

#endif // JPLATFORM_WINDOWS

#endif // _JOJ_WIN32_TIMER_H