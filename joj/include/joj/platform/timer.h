#ifndef _JOJ_TIMER_H
#define _JOJ_TIMER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

namespace joj
{
    class JAPI Timer
    {
    public:
        Timer();
        virtual ~Timer();

        void start() {}
        void stop() {}

        virtual f32 reset() = 0;
        virtual f32 elapsed() = 0;
        virtual b8 was_elapsed(f32 secs) = 0;

        f64 get_total_elapsed() const;

        void time_begin_period() {}
        void time_end_period() {}

    protected:
        b8 m_stopped;
        f64 m_cumulative_elapsed;
    };

    inline f64 Timer::get_total_elapsed() const
    { return m_cumulative_elapsed; }
}

#endif // _JOJ_TIMER_H