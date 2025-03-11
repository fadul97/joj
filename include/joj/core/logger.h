/**
 * @file logger.h
 * @author Leonardo Fadul (lsffadul@gmail.com)
 * @brief This file contains the terminal logging system for the engine.
 * @version 0.1
 * @date 2025-03-11
 * 
 * @copyright Copyright (c) 2025
 * 
 * REFERENCES: Travis Vroman (https://travisvroman.com/) - Kohi Game Engine (https://github.com/travisvroman/kohi)
 * 
 */

#ifndef _JOJ_LOGGER_H
#define _JOJ_LOGGER_H

#include "defines.h"
#include "error_code.h"

/** @brief Change to 1 when exporting. */
#define JOJ_RELEASE 0

/** @brief Disables Warn, Debug and Info in Release Mode. */
#if JOJ_RELEASE == 1
#define JOJ_LOG_WARN_ENABLED 0
#define JOJ_LOG_DEBUG_ENABLED 0
#define JOJ_LOG_INFO_ENABLED 0
#define JOJ_LOG_TODO 0
#else

/** @brief Indicates if warning level logging is enabled. */
#define JOJ_LOG_WARN_ENABLED 1

/** @brief Indicates if debug level logging is enabled */
#define JOJ_LOG_DEBUG_ENABLED 1

/** @brief Indicates if info level logging is enabled. */
#define JOJ_LOG_INFO_ENABLED 1

/** @brief Indicates if todo logging is enabled. */
#define JOJ_LOG_TODO 1

#endif

namespace joj
{
    /** @brief Represents logging levels. */
    enum class LogLevel : i32
    {
        /** @brief Fatal log level, should be used to stop the application when hit. */
        LOG_LEVEL_FATAL = 0,

        /** @brief Error log level, should be used to indicate critical runtime problems that cause the application to run improperly or not at all. */
        LOG_LEVEL_ERROR = 1,

        /** @brief Warning log level, should be used to indicate non-critial problems with the application that cause it to run suboptimally. */
        LOG_LEVEL_WARN = 2,

        /** @brief Debug log level, should be used for debugging purposes. */
        LOG_LEVEL_DEBUG = 3,

        /** @brief Info log level, should be used for non-erronuous informational purposes. */
        LOG_LEVEL_INFO = 4,

        /** @brief Todo log level, should be used to spot non-implemented code. */
        LOG_LEVEL_TODO = 5,
    };

    namespace Logger
    {
        /**
         * @brief Writes the log to the console. This is called when the macro is used.
         * 
         * @param level The log level to use.
         * @param err The error code to be logged.
         * @param file The file where the log was called.
         * @param line The line where the log was called.
         * @param message The message to be logged.
         * @param ... Any additional arguments (data) to be logged.
         */
        JOJ_API void log(LogLevel level, joj::ErrorCode err, const char* file,
            i32 line, const char* message, ...);
    }
}

#if JOJ_LOG_TODO == 1
/**
 * @brief Logs an TODO message. Should be used for non-implemented code blocks.
 */
#define JOJ_TODO() joj::Logger::log(joj::LogLevel::LOG_LEVEL_TODO, joj::ErrorCode::OK,  __FILE__, __LINE__, "[TODO]: %s", __FUNCTION__);
#else
#define JOJ_TODO();
#endif // LOG_TODO

#if JOJ_LOG_INFO_ENABLED == 1
/**
 * @brief Logs an info-level message. Should be used for non-erronuous informational purposes.
 * @param message The message to be logged.
 * @param ... Any additional arguments (data) to be logged.
 */
#define JOJ_INFO(message, ...) joj::Logger::log(joj::LogLevel::LOG_LEVEL_INFO, joj::ErrorCode::OK, __FILE__, __LINE__, message, ##__VA_ARGS__);
#else
#define JOJ_INFO(message, ...);
#endif

#if JOJ_LOG_DEBUG_ENABLED == 1
/**
 * @brief Logs a debug-level message. Should be used for debugging purposes.
 * @param message The message to be logged.
 * @param ... Any additional arguments (data) to be logged.
 */
#define JDEBUG(message, ...) joj::Logger::log(joj::LogLevel::LOG_LEVEL_DEBUG, joj::ErrorCode::OK, __FILE__, __LINE__, message, ##__VA_ARGS__);
#else
#define JDEBUG(message, ...);
#endif

#if JOJ_LOG_WARN_ENABLED == 1
/**
 * @brief Logs a warning-level message. Should be used to indicate non-critial problems that cause the application to run inefficient.
 * @param message The message to be logged.
 * @param ... Any additional arguments (data) to be logged.
 */
#define JOJ_WARN(message, ...) joj::Logger::log(joj::LogLevel::LOG_LEVEL_WARN, joj::ErrorCode::OK,  __FILE__, __LINE__, message, ##__VA_ARGS__);
#else
#define JOJ_WARN(message, ...);
#endif

#ifndef JOJ_ERROR
/**
 * @brief Logs an error-level message. Should be used to indicate runtime problems.
 * @param error The error code to be logged.
 * @param message The message to be logged.
 * @param ... Any additional arguments (data) to be logged.
 */
#define JOJ_ERROR(error, message, ...) joj::Logger::log(joj::LogLevel::LOG_LEVEL_ERROR, error, __FILE__, __LINE__, message, ##__VA_ARGS__);
#endif

#ifndef JOJ_FATAL
/**
 * @brief Logs a fatal-level message. Should be used to stop the application when hit.
 * @param error The error code to be logged.
 * @param message The message to be logged.
 * @param ... Any additional arguments (data) to be logged.
 */
#define JOJ_FATAL(error, message, ...) joj::Logger::log(joj::LogLevel::LOG_LEVEL_FATAL, error, __FILE__, __LINE__, message, ##__VA_ARGS__);
#endif

#endif // _JOJ_LOGGER_H
