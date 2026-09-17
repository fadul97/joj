#ifndef _JOJ_LOGGER_HPP
#define _JOJ_LOGGER_HPP

#include "joj/core/types.h"

// STD Includes
#include <stdarg.h>

namespace joj {

enum class LogLevel : u8 {
    NONE = 0,
    TRACE,
    INFO,
    DEBUG,
    TODO,
    WARN,
    ERROR,
    FATAL,
    MAX
};

void log(LogLevel const level, char const* file, u32 const line, char const* function, char const* format, ...);

} // namespace joj

#if JOJ_MODE_DEBUG
#define JOJ_LOG_TRACE(msg, ...) joj::log(joj::LogLevel::TRACE, __FILE__, __LINE__, __FUNCTION__, msg, __VA_ARGS__);
#define JOJ_TODO() joj::log(joj::LogLevel::TODO, __FILE__, __LINE__, __FUNCTION__, "");
#else
#define JOJ_TODO() joj::log(joj::LogLevel::TODO, __FILE__, __LINE__, __FUNCTION__, "");
#endif

#endif // _JOJ_LOGGER_HPP
