#ifndef _JOJ_TOKEN_TYPE_H
#define _JOJ_TOKEN_TYPE_H

#include "keywords.h"
#include <string>

namespace joj
{
    // var x : int = 10;
    // var y = 10;

    enum class TokenType
    {
        UNKNOWN,
        END_OF_FILE,
        VAR_DECLARATION,
        ID,
        COLON,
        INT,
        FLOAT,
        CHAR,
        BOOLEAN,
        STRING,
        ASSIGN,
        VALUE_INT,
        VALUE_FLOAT,
        VALUE_CHAR,
        VALUE_STRING,
        VALUE_BOOLEAN,
        SEMICOLON,
        FUNC_DECLARATION,
        MAIN_FUNC,
        IF,
        ELSE,
        ELSE_IF,
        MATCH,
        FOR,
        IN,
        WHILE,
        LOOP,
        BREAK,
        TRUE,
        FALSE,
        EQUALS,
        NOT_EQUAL,
        GREATER_THAN,
        LESS_THAN,
        AND,
        OR,
        ADD_SIGN,
        SUB_SIGN,
        DIV_SIGN,
        MUL_SIGN,
        OPEN_PAR,
        CLOSE_PAR,
        OPEN_BRAC,
        CLOSE_BRAC,
        OPEN_CURLY_BRAC,
        CLOSE_CURLY_BRAC,
        COUNTER_BAR,
        EXCLAMATION,
        PIPE
    };

    inline TokenType string_to_tokentype(const std::string& str)
    {
        if (str == KEYWORD_CHAR)
        {
            return TokenType::CHAR;
        }
        else if (str == KEYWORD_INT)
        {
            return TokenType::INT;
        }
        else if (str == KEYWORD_FLOAT)
        {
            return TokenType::FLOAT;
        }
        else if (str == KEYWORD_STRING)
        {
            return TokenType::STRING;
        }
        else
        {
            return TokenType::UNKNOWN;
        }
    }
}


#endif // _JOJ_TOKEN_TYPE_H