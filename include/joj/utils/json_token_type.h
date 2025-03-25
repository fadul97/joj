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
        Boolean,
        Null, 
        EndOfFile,
        Error
    };
}

#endif // _JOJ_JSON_TOKEN_TYPE_H