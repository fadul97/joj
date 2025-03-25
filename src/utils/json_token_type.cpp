#include "joj/utils/json_token_type.h"

const char* joj::json_token_type_to_string(const JsonTokenType type)
{
    switch (type)
    {
    case JsonTokenType::LeftBrace:      return "LeftBrace";
    case JsonTokenType::RightBrace:     return "RightBrace";
    case JsonTokenType::LeftBracket:    return "LeftBracket";
    case JsonTokenType::RightBracket:   return "RightBracket";
    case JsonTokenType::Colon:          return "Colon";
    case JsonTokenType::Comma:          return "Comma";
    case JsonTokenType::String:         return "String";
    case JsonTokenType::Number:         return "Number";
    case JsonTokenType::Boolean:        return "Boolean";
    case JsonTokenType::Null:           return "Null";
    case JsonTokenType::EndOfFile:      return "EndOfFile";
    case JsonTokenType::Error:          return "Error";
    default:                            return "Unknown";
    }
}