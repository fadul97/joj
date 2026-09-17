#include "joj/core/error/error_code.hpp"

namespace joj {

char const* error_code_to_cstr(ErrorCode const code)
{
    switch (code)
    {
    case ErrorCode::OK:
        return "OK";

#define X(err)           \
    case ErrorCode::err: \
        return #err;

        LIST_OF_ERROR_CODES

#undef X

    default:
        return "UNKNOWN";
    }
}

} // namespace joj
