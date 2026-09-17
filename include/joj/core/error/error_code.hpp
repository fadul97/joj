#ifndef _JOJ_ERROR_CODE_HPP
#define _JOJ_ERROR_CODE_HPP

#include "joj/core/types.h"

#undef X
#define LIST_OF_ERROR_CODES                           \
    /** @brief Generic error code. */                 \
    X(FAILED)                                         \
                                                      \
    /** @brief Used when assertion fails. */          \
    X(ASSERTION_FAILED)                               \
                                                      \
    /** @brief Used to signal maximum error value. */ \
    X(MAX)

namespace joj {

enum class ErrorCode : u8 {
    /** @brief Used for successfull operations. */
    OK = 0,

#define X(err) err,
    LIST_OF_ERROR_CODES
#undef X
};

char const* error_code_to_cstr(ErrorCode const code);

} // namespace joj

#endif // _JOJ_ERROR_CODE_HPP
