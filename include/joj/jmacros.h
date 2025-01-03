#ifndef _JOJ_MACROS_H
#define _JOJ_MACROS_H

#include "error_code.h"
#include "logger.h"

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

#else // JOJ_DEBUG_MODE not defined (Release mode)

#define JOJ_FAILED(x)
#define JOJ_RETURN_INT_IF_FAIL(x)
#define JOJ_RETURN_INT_IF_FAIL(x)

#endif // JOJ_DEBUG_MODE

#endif // _JOJ_MACROS_H