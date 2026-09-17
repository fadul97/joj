#include "joj/core/logging/logger.hpp"

#include "joj/core/assert.hpp"
#include <stdio.h>

namespace joj {

void output_log(LogLevel const level, char const* file, u32 const line, char const* function, char const* format, va_list& args)
{
    constexpr u32 BUFFER_LEN = 1000;

    char const* log_level[static_cast<u32>(LogLevel::MAX)]{
        "",
        "[TRACE]",
        "[INFO]",
        "[DEBUG]",
        "[TODO]",
        "[WARN]",
        "[ERROR]",
        "[FATAL]",
    };

    [[maybe_unused]] char const* colors[static_cast<u32>(LogLevel::MAX)]{
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
    };

    u32 const lvl = static_cast<u32>(level);

    if (level == LogLevel::TODO)
    {
        char buf[BUFFER_LEN]{};
        char tmp[BUFFER_LEN]{}; // TODO(leonardo): Is second buffer necessary?
        sprintf(tmp, "[%s - %d] %s: %s\n", file, line, log_level[lvl], function);
        vsnprintf(buf, BUFFER_LEN - 1, tmp, args);
        printf("%s", buf);
        return;
    }

    char buf[BUFFER_LEN]{};
    char tmp[BUFFER_LEN]{}; // TODO(leonardo): Is second buffer necessary?

    sprintf(tmp, "[%s - %d: %s]: %s", file, line, function, format);
    vsnprintf(buf, BUFFER_LEN - 1, tmp, args);

    printf("%s", buf);
}

void log(LogLevel const level, char const* file, u32 const line, char const* function, char const* format, ...)
{
    JOJ_ASSERT(file);
    JOJ_ASSERT(function);

    va_list args;
    va_start(args, format);
    output_log(level, file, line, function, format, args);
    va_end(args);
}

} // namespace joj
