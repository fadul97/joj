#ifndef _JOJ_LOGGER_HPP
#define _JOJ_LOGGER_HPP

#include "joj/core/types.h"

namespace joj {

enum class LogLevel : u8 {
    NONE = 0,
    TRACE,
    INFO,
    DEBUG,
    TODO,
    WARN,
    ERROR,
    FATAL
};

void log(LogLevel const level, ...);

} // namespace joj

#endif // _JOJ_LOGGER_HPP
