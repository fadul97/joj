#include "joj/core/logger.h"

#if JOJ_PLATFORM_LINUX

#define JERROR_MSG_MAX_LEN 256

#include "joj/core/error_code.h"
#include "joj/core/jmacros.h"

#include <cstdio>
#include <cstdarg>

namespace joj::Logger
{
    /**
     * @brief Error message to be used in the terminal logging system.
     * This message is only displayed in the terminal.
     *
     * @note Internal use only.
     *
     * @note It should not be used for long messages! See explanation below.
     *
     * Using a fixed-size array of 256 bytes instead of a pointer (8 bytes - 64bits CPU),
     * to avoid memory allocations when logging to the terminal. Since error messages
     * should (FOR NOW) be short and concise, dynamic allocation is unnecessary.
     *
     *  Total size: 256 bytes.
     */
    struct JErrorMessage
    {
        /** @brief The message itself. Size: 256 bytes. */
        char msg[JERROR_MSG_MAX_LEN];
    };

    /**
     * @brief Initializea the message array.
     * @param err_msg The JErrorMessage instance to be initialized.
     */
    void jerror_msg_init(JErrorMessage* err_msg)
    {
        JOJ_ASSERT(err_msg, "err_msg is nullptr");

        // TODO: Use a more efficient method - maybe memset
        for (i32 i = 0; i < JERROR_MSG_MAX_LEN; ++i)
            err_msg->msg[i] = '\0';
    }

    /**
     * @brief Appends a message to the JErrorMessage output.
     * @param err_msg The JErrorMessage instance where the message will be appended.
     * @param src The message to be appended at the end of err_msg.
     */
    void jerror_msg_concat(JErrorMessage* err_msg, const char* src)
    {
        JOJ_ASSERT(err_msg, "err_msg is nullptr");
        JOJ_ASSERT(src, "src is nullptr");

        // Get first value of the error msg
        char* aux = err_msg->msg;

        // Move aux to the end of the string
        while (*aux)
            ++aux;

        // Append src to error message without modifying src
        while (*src)
            *aux++ = *src++;
    }

    /**
     * TODO: Use different colors for each log.
     *
     * @brief Outputs logging at the given level.
     *
     * @note Internal use only. It is recommended to use the macros defined
     * in the header file (JINFO, JDEBUG, JWARN, JERROR, JFATAL).
     *
     * @param level The log level to use.
     * @param err The error code to be logged.
     * @param file The file where the log was called.
     * @param line The line where the log was called.
     * @param message The message to be logged.
     * @param args Any additional arguments (data) to be logged.
     */
    void write_log(const LogLevel level, const ErrorCode err,
                   const char* file, const i32 line, const char* message,
                   va_list args)
    {
        const char* level_strings[5] =
        {
            "[FATAL]: ", "[ERROR]: ", "[WARN]: ", "[DEBUG]: ", "[INFO]: "
        };

        const char* err_name = err_to_str(err);

        // Cast ErrorCode to i32
        const i32 level_index = static_cast<i32>(level);

        // Log TODO
        if (level == LogLevel::LOG_LEVEL_TODO)
        {
            // Prepare the log message with file and line information
            JErrorMessage full_msg;
            snprintf(full_msg.msg, sizeof(full_msg.msg), "%s\nFile: %s\nLine: %d\n", message, file, line);
            vprintf(full_msg.msg, args);
            return;
        }


        // log WARN
        if (level == LogLevel::LOG_LEVEL_WARN)
        {
            // Prepare the log message with file and line information
            JErrorMessage full_msg;
            snprintf(full_msg.msg, sizeof(full_msg.msg), "%sWARN - %s\nFile: %s\nLine: %d\n", level_strings[level_index], message, file, line);
            vprintf(full_msg.msg, args);
            return;
        }

        JErrorMessage full_msg;
        snprintf(full_msg.msg, sizeof(full_msg.msg), "%s%s - %s\nFile: %s\nLine: %d\n", level_strings[level_index], err_name, message, file, line);
        vprintf(full_msg.msg, args);
    }
}

void joj::Logger::log(const LogLevel level, const ErrorCode err, const char* file,
                      const i32 line, const char* message, ...)
{
    va_list args;
    va_start(args, message);
    write_log(level, err, file, line, message, args);
    va_end(args);
}

#endif // JOJ_PLATFORM_LINUX
