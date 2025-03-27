#ifndef _JOJ_JSON_TOKEN_TYPE_H
#define _JOJ_JSON_TOKEN_TYPE_H

#include "joj/core/defines.h"

namespace joj
{
    enum class JsonTokenType
    {
        LeftBrace,
        RightBrace,
        LeftBracket,
        RightBracket,
        Colon,
        Comma,
        String,
        Number,
        Integer,
        Float,
        Boolean,
        Null, 
        EndOfFile,
        Error
    };

    JOJ_API const char* json_token_type_to_string(const JsonTokenType type);
}

#endif // _JOJ_JSON_TOKEN_TYPE_H