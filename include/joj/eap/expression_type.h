#ifndef _JOJ_EXPRESSION_TYPE_H
#define _JOJ_EXPRESSION_TYPE_H

#include "keywords.h"
#include <string>

namespace joj
{
    enum class ExpressionType
    {
        VOID,
        INT,
        FLOAT,
        BOOL
    };

    inline const char* exprtype_to_string(const ExpressionType etype)
    {
        switch (etype)
        {
        case ExpressionType::VOID:  return KEYWORD_VOID;
        case ExpressionType::INT:   return KEYWORD_INT;
        case ExpressionType::FLOAT: return KEYWORD_FLOAT;
        case ExpressionType::BOOL:  return KEYWORD_BOOL;
        default:                    return KEYWORD_VOID;
        }
    }

    inline ExpressionType string_to_exprtype(const std::string& lexeme)
    {
        if (lexeme == KEYWORD_VOID)
            return ExpressionType::VOID;

        if (lexeme == KEYWORD_INT)
            return ExpressionType::INT;

        if (lexeme == KEYWORD_FLOAT)
            return ExpressionType::FLOAT;

        if (lexeme == KEYWORD_BOOL)
            return ExpressionType::BOOL;

        return ExpressionType::VOID;
    }
}

#endif // _JOJ_EXPRESSION_TYPE_H