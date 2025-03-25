#ifndef _JOJ_JSON_PARSER_H
#define _JOJ_JSON_PARSER_H

#include "joj/core/defines.h"

#include "json_value.h"

namespace joj
{
    namespace JsonParser
    {
        JOJ_API void hello_world();
        JOJ_API JsonValue parse(const std::string& json);
    }
}

#endif // _JOJ_JSON_PARSER_H