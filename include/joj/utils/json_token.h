#ifndef _JOJ_JSON_TOKEN_H
#define _JOJ_JSON_TOKEN_H

#include "joj/core/defines.h"

#include "json_token_type.h"
#include <string>

namespace joj
{
    /**
     * @brief A struct that represents a JSON token.
     * The token has a type and a value.
     */
    struct JOJ_API JsonToken
    {
        JsonTokenType type;
        std::string value;
    };
}

#endif // _JOJ_JSON_TOKEN_H