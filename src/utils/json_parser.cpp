#include "joj/utils/json_parser.h"

#include <iostream>

joj::JsonParser::JsonParser(std::string_view json)
    : m_lexer(json), m_error_count(0)
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
        ++m_error_count;
        std::cerr << "Error: Invalid token type: " << m_current_token.value << std::endl;
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
            ++m_error_count;
            std::cerr << "Error: Expected key name." << std::endl;
            return JsonValue();
        }

        // Parse key
        std::string key = m_current_token.value;
#if JOJ_DEBUG_MODE
        // std::cout << "[" << json_token_type_to_string(m_current_token.type) << "]: " << m_current_token.value << std::endl;
#endif
        // Skip key
        advance();

        // Print token type and value

        if (m_current_token.type != JsonTokenType::Colon)
        {
            ++m_error_count;
            std::cerr << "Error: Expected ':' after key '" << key << "'." << std::endl;
            return JsonValue();
        }

        // Skip ':'
        advance();

        // Maybe there is something wrong with they way I keep adding values to the object
        // Parse value
        object[key] = parse_value();

#if JOJ_DEBUG_MODE
        // object[key].print();
#endif

        if (m_current_token.type == JsonTokenType::RightBrace)
            continue;

        if (m_current_token.type == JsonTokenType::RightBracket)
            advance();

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
        JsonValue value = parse_value();

        if (m_current_token.type == JsonTokenType::RightBracket)
            continue;

        array.push_back(value);

        // Consume value
        advance();

        // Skip ','
        if (m_current_token.type == JsonTokenType::Comma)
            advance();

        if (m_current_token.type == JsonTokenType::LeftBrace)
            continue;
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

    ++m_error_count;
    std::cerr << "Error: Invalid keyword: " << m_current_token.value << std::endl;
    return JsonValue();
}

u32 joj::JsonParser::get_error_count()
{
    return m_lexer.get_error_count() + m_error_count;
}