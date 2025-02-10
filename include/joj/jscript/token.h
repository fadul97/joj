#ifndef _JOJ_TOKEN_H
#define _JOJ_TOKEN_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "core/defines.h"

#include "token_type.h"
#include <string>

namespace joj
{
    struct Token
    {
        TokenType type = TokenType::UNKNOWN;
        std::string lexeme;
    };

    inline const char* tokentype_to_string(const TokenType t)
    {
        switch (t)
        {
        case TokenType::VAR_DECLARATION:  return "VAR_DECLARATION";
        case TokenType::ID:               return "ID";
        case TokenType::COLON:            return "COLON";
        case TokenType::INT:              return "INT";
        case TokenType::FLOAT:            return "FLOAT";
        case TokenType::CHAR:             return "CHAR";
        case TokenType::BOOLEAN:          return "BOOLEAN";
        case TokenType::STRING:           return "STRING";
        case TokenType::ASSIGN:           return "ASSIGN";
        case TokenType::NUMBER_INT:       return "NUMBER_INT";
        case TokenType::NUMBER_FLOAT:     return "NUMBER_FLOAT";
        case TokenType::SEMICOLON:        return "SEMICOLON";
        case TokenType::FUNC_DECLARATION: return "FUNC_DECLARATION";
        case TokenType::MAIN_FUNC:        return "MAIN_FUNC";
        case TokenType::IF:               return "IF";
        case TokenType::ELSE:             return "ELSE";
        case TokenType::ELSE_IF:          return "ELSE_IF";
        case TokenType::MATCH:            return "MATCH";
        case TokenType::FOR:              return "FOR";
        case TokenType::IN:               return "IN";
        case TokenType::WHILE:            return "WHILE";
        case TokenType::LOOP:             return "LOOP";
        case TokenType::BREAK:            return "BREAK";
        case TokenType::TRUE:             return "TRUE";
        case TokenType::FALSE:            return "FALSE";
        case TokenType::EQUALS:           return "EQUALS";
        case TokenType::NOT_EQUAL:        return "NOT_EQUAL";
        case TokenType::GREATER_THAN:     return "GREATER_THAN";
        case TokenType::LESS_THAN:        return "LESS_THAN";
        case TokenType::AND:              return "AND";
        case TokenType::OR:               return "OR";
        case TokenType::ADD_SIGN:         return "ADD_SIGN";
        case TokenType::SUB_SIGN:         return "SUB_SIGN";
        case TokenType::DIV_SIGN:         return "DIV_SIGN";
        case TokenType::MUL_SIGN:         return "MUL_SIGN";
        case TokenType::OPEN_PAR:         return "OPEN_PAR";
        case TokenType::CLOSE_PAR:        return "CLOSE_PAR";
        case TokenType::OPEN_BRAC:        return "OPEN_BRAC";
        case TokenType::CLOSE_BRAC:       return "CLOSE_BRAC";
        case TokenType::OPEN_CURLY_BRAC:  return "OPEN_CURLY_BRAC";
        case TokenType::CLOSE_CURLY_BRAC: return "CLOSE_CURLY_BRAC";
        case TokenType::COUNTER_BAR:      return "COUNTER_BAR";
        case TokenType::PIPE:             return "PIPE";
        case TokenType::END_OF_FILE:      return "END_OF_FILE";
        default:                          return "UNKNOWN";
        }
    }
}

#endif // _JOJ_TOKEN_H