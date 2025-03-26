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
    case JsonTokenType::Boolean: return parse_bool();
    case JsonTokenType::Null: return parse_null();
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
        advance(); // Skip key

        if (m_current_token.type != JsonTokenType::Colon)
        {
            ++m_error_count;
            std::cerr << "Error: Expected ':' after key '" << key << "'." << std::endl;
            return JsonValue();
        }

        // Skip ':'
        advance();

        // Parse value
        object[key] = parse_value();

        // Verifica se há vírgula ou fim do objeto
        if (m_current_token.type == JsonTokenType::Comma)
        {
            advance(); // Skip ','
        }
        else if (m_current_token.type != JsonTokenType::RightBrace)
        {
            ++m_error_count;
            std::cerr << "Error: Expected ',' or '}' after key-value pair." << std::endl;
            return JsonValue();
        }
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

        // Se o próximo token for ',', continue para o próximo valor
        if (m_current_token.type == JsonTokenType::Comma)
        {
            advance();
        }
        else if (m_current_token.type != JsonTokenType::RightBracket)
        {
            ++m_error_count;
            std::cerr << "Error: Expected ',' or ']' after array value." << std::endl;
            return JsonValue();
        }
    }

    // Skip ']'
    advance();

    return JsonValue(array);
}

joj::JsonValue joj::JsonParser::parse_bool()
{
    if (m_current_token.value == "true")
    {
        advance();
        return JsonValue(true);
    }
    else if (m_current_token.value == "false")
    {
        advance();
        return JsonValue(false);
    }

    // TODO: Log an error?
    static const JsonValue null_value;
    return null_value;
}

joj::JsonValue joj::JsonParser::parse_null()
{
    if (m_current_token.value == "null")
    {
        advance();
        return JsonValue();
    }

    // TODO: Log an error?
    static const JsonValue null_value;
    return null_value;
}

joj::JsonValue joj::JsonParser::parse_string()
{
    const std::string str = m_current_token.value;
    advance();
    return JsonValue(str);
}

joj::JsonValue joj::JsonParser::parse_number()
{
    const std::string number = m_current_token.value;
    advance();
    return JsonValue(std::stod(number));
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