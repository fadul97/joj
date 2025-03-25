#include "joj/utils/json_parser.h"

#include <iostream>

joj::JsonParser::JsonParser(std::string_view json)
    : m_lexer(json)
{
    advance();
}

joj::JsonParser::~JsonParser()
{
}

joj::JsonValue joj::JsonParser::parse()
{
    return parse_value();
}

void joj::JsonParser::advance()
{
    m_current_token = m_lexer.next_token();
}

joj::JsonValue joj::JsonParser::parse_value()
{
    switch (m_current_token.type)
    {
    case JsonTokenType::LeftBrace: return parse_object();
    case JsonTokenType::LeftBracket: return parse_array();
    case JsonTokenType::String: return parse_string();
    case JsonTokenType::Number: return parse_number();
    case JsonTokenType::Boolean: return { m_current_token.value == "true" };
    case JsonTokenType::Null: return JsonValue();
    default:
        std::cerr << "Error: Invalid token type: " << static_cast<u32>(m_current_token.type) << std::endl;
        return JsonValue();
    }
}

joj::JsonValue joj::JsonParser::parse_object()
{
    JsonValue::Object object;

    // Skip '{'
    advance();
    
    while (m_current_token.type != JsonTokenType::RightBrace)
    {
        if (m_current_token.type != JsonTokenType::String)
        {
            std::cerr << "Error: Expected key name." << std::endl;
            return JsonValue();
        }

        // Parse key
        std::string key = m_current_token.value;
        // Skip key
        advance();

        if (m_current_token.type != JsonTokenType::Colon)
        {
            std::cerr << "Error: Expected ':' after key." << std::endl;
            return JsonValue();
        }

        // Skip ':'
        advance();

        // Parse value
        object[key] = parse_value();

        // Consume ','
        if (m_current_token.type != JsonTokenType::Comma)
            advance();

        // Skip ','
        if (m_current_token.type == JsonTokenType::Comma)
            advance();
    }

    // Skip '}'
    advance();

    return JsonValue(object);
}

joj::JsonValue joj::JsonParser::parse_array()
{
    JsonValue::Array array;

    // Skip '['
    advance();

    while (m_current_token.type != JsonTokenType::RightBracket)
    {
        // Parse value
        array.push_back(parse_value());

        // Consume ','
        advance();

        // Skip ','
        if (m_current_token.type == JsonTokenType::Comma)
            advance();
    }

    // Skip ']'
    advance();

    return JsonValue(array);
}

joj::JsonValue joj::JsonParser::parse_string()
{
    return JsonValue(m_current_token.value);
}

joj::JsonValue joj::JsonParser::parse_number()
{
    return JsonValue(std::stod(m_current_token.value));
}

joj::JsonValue joj::JsonParser::parse_keyword()
{
    if (m_current_token.value == "true")
        return JsonValue(true);
    else if (m_current_token.value == "false")
        return JsonValue(false);
    else if (m_current_token.value == "null")
        return JsonValue();

    return JsonValue();
}