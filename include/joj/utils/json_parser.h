#ifndef _JOJ_JSON_PARSER_H
#define _JOJ_JSON_PARSER_H

#include "joj/core/defines.h"

#include <string_view>
#include "json_lexer.h"
#include "json_value.h"

namespace joj
{
    class JOJ_API JsonParser
    {
    public:
        JsonParser(std::string_view json);
        ~JsonParser();

        JsonValue parse();

        void advance();

        u32 get_error_count();

    private:
        JsonLexer m_lexer;
        JsonToken m_current_token;

        u32 m_error_count;

        JsonValue parse_value();
        JsonValue parse_object();
        JsonValue parse_array();
        JsonValue parse_string();
        JsonValue parse_number();
        JsonValue parse_keyword();
    };
}

#endif // _JOJ_JSON_PARSER_H