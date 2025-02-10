#ifndef _JOJ_MACROS_H
#define _JOJ_MACROS_H

#include "core/error_code.h"
#include "core/logger.h"

#if JOJ_DEBUG_MODE

#ifndef JOJ_FAILED
#define JOJ_FAILED(x) (((joj::ErrorCode)(x)) != joj::ErrorCode::OK)
#endif // JFAILED

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

#ifndef JOJ_RETURN_INT_IF_FAIL
#define JOJ_RETURN_INT_IF_FAIL(x)                                                                                  \
{                                                                                                                  \
    joj::ErrorCode result = (x);                                                                                   \
    if(result != joj::ErrorCode::OK) {                                                                             \
        joj::Logger::log(joj::LogLevel::LOG_LEVEL_ERROR, result, __FILE__, __LINE__, "Function: %s", __func__);    \
        return joj::err_to_int(result);                                                                            \
    }                                                                                                              \
}
#endif // JOJ_RETURN_INT_IF_FAIL

#ifndef JOJ_RETURN_VOID_IF_FAIL
#define JOJ_RETURN_VOID_IF_FAIL(x)                                                                                 \
{                                                                                                                  \
    joj::ErrorCode result = (x);                                                                                   \
    if(result != joj::ErrorCode::OK) {                                                                             \
        joj::Logger::log(joj::LogLevel::LOG_LEVEL_ERROR, result, __FILE__, __LINE__, "Function: %s", __func__);    \
        return;                                                                                                    \
    }                                                                                                              \
}
#endif // JOJ_RETURN_VOID_IF_FAIL

#ifndef JOJ_ASSERT
#define JOJ_ASSERT(condition, ...)                                                                                 \
    do {                                                                                                           \
        if (!(condition)) {                                                                                        \
            joj::Logger::log(joj::LogLevel::LOG_LEVEL_ERROR, joj::ErrorCode::ASSERTION_FAILED,                     \
                    __FILE__, __LINE__, "Assertion failed in function: %s | Message: " __VA_ARGS__, __func__);     \
            std::terminate(); /* Encerra o programa em caso de falha crítica */                                    \
        }                                                                                                          \
    } while (0)
#endif // JOJ_ASSERT

#else // JOJ_DEBUG_MODE not defined (Release mode)

#define JOJ_FAILED(x) (((joj::ErrorCode)(x)) != joj::ErrorCode::OK)
#define JOJ_LOG_IF_FAIL(x) (void)(x)
#define JOJ_RETURN_INT_IF_FAIL(x) (void)(x)
#define JOJ_RETURN_VOID_IF_FAIL(x) (void)(x)
#define JOJ_ASSERT(condition, ...)

#endif // JOJ_DEBUG_MODE

#endif // _JOJ_MACROS_H