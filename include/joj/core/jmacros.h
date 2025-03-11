#ifndef _JOJ_JMACROS_H
#define _JOJ_JMACROS_H

#include "defines.h"
#include "error_code.h"
#include "logger.h"
#include <stdio.h>

#if JOJ_DEBUG_MODE

extern "C" void exit(int status);

/**
 * @brief Macro to mark a variable as unused.
 */
#define JOJ_UNUSED(x) (void)(x)

/**
 * @brief Macro to check if a function returned an error code different from OK.
 */
#ifndef JOJ_FAILED
#define JOJ_FAILED(x) (((joj::ErrorCode)(x)) != joj::ErrorCode::OK)
#endif // JFAILED

/**
 * @brief Macro to assert a condition and log an error if the condition is false.
 * TODO: Shutdown program in critical failure
 */
#ifndef JOJ_ASSERT
#define JOJ_ASSERT(condition, ...)                                                                                 \
    do {                                                                                                           \
        if (!(condition)) {                                                                                        \
            printf("Assertion failed in function: %s | Message: " __VA_ARGS__, __func__);                          \
            exit(1);                                                                                               \
        }                                                                                                          \
    } while (0)
#endif // JOJ_ASSERT

#ifndef JOJ_LOG_IF_FAIL
#define JOJ_LOG_IF_FAIL(x)                                                                                         \
{                                                                                                                  \
    joj::ErrorCode result = (x);                                                                                   \
    if(result != joj::ErrorCode::OK) {                                                                             \
        joj::Logger::log(joj::LogLevel::LOG_LEVEL_ERROR, result, __FILE__, __LINE__, "Function: %s", __func__);    \
        /*return result;*/                                                                                         \
    }                                                                                                              \
}
#endif // JOJ_LOG_IF_FAIL

#else // JOJ_DEBUG_MODE not defined (Release mode)

#define JOJ_UNUSED(x) (void)(x)
#define JOJ_FAILED(x) (((joj::ErrorCode)(x)) != joj::ErrorCode::OK)
#define JOJ_LOG_IF_FAIL_AND_RETURN_ERROR(x) x
#define JOJ_ASSERT(condition, ...)
#define JOJ_LOG_IF_FAIL(x) (void)(x)

#endif // JOJ_DEBUG_MODE

#endif // _JOJ_JMACROS_H
