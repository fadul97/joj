#ifndef _JOJ_EXPRESSION_TYPE_H
#define _JOJ_EXPRESSION_TYPE_H

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
        case ExpressionType::VOID:  return "void";
        case ExpressionType::INT:   return "int";
        case ExpressionType::FLOAT: return "dec";
        case ExpressionType::BOOL:  return "bool";
        default: return "void";
        }
    }
}

#endif // _JOJ_EXPRESSION_TYPE_H